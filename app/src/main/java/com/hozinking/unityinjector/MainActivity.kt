package com.hozinking.unityinjector

import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import androidx.appcompat.app.AppCompatActivity
import com.hozinking.unityinjector.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Inject is enabled as soon as a package name is entered: the
        // auto-launch flow covers the game-not-running case, so a PID is
        // no longer a prerequisite (v1.0.1's guard caused the exit=1 tap).
        binding.btnInject.isEnabled = false
        val injectEnabler = object : TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {}
            override fun afterTextChanged(s: Editable?) {
                binding.btnInject.isEnabled =
                    binding.editProcessName.text.toString().trim().isNotEmpty()
            }
        }
        binding.editProcessName.addTextChangedListener(injectEnabler)

        binding.btnCheckRoot.setOnClickListener {
            Thread {
                val r = InjectorBridge.checkRoot()
                log("root check: exit=${r.exitCode} output=${r.output}")
                if (r.exitCode == 0 && r.output.contains("uid=0")) {
                    log("root: OK (uid=0)")
                } else {
                    log("Root ditolak — buka aplikasi KernelSU/Magisk, izinkan root untuk Unity Controller Injector, lalu coba lagi")
                }
            }.start()
        }

        binding.btnShowPaths.setOnClickListener {
            log("injector: ${InjectorBridge.injectorPath(this)}")
            log("payload:  ${InjectorBridge.payloadPath(this)}")
        }

        binding.btnFindPid.setOnClickListener {
            val target = binding.editProcessName.text.toString().trim()
            if (target.isEmpty()) {
                log("enter a package / process name first")
                return@setOnClickListener
            }
            Thread {
                val pid = InjectorBridge.findPid(target)
                if (pid != null) {
                    runOnUiThread {
                        binding.editPid.setText(pid.toString())
                        binding.btnInject.isEnabled = true
                    }
                    log("found pid $pid for $target")
                } else {
                    log("no running process matched $target")
                }
            }.start()
        }

        binding.btnInject.setOnClickListener {
            val target = binding.editProcessName.text.toString().trim()
            if (target.isEmpty()) {
                log("isi nama paket game dulu")
                return@setOnClickListener
            }
            val typedPid = binding.editPid.text.toString().trim().toIntOrNull()
            Thread {
                // 1. Game belum jalan -> buka otomatis; sudah jalan -> lanjut.
                var pid = typedPid ?: InjectorBridge.findPid(target)
                if (pid == null) {
                    log("game belum jalan — buka otomatis: $target")
                    val via = InjectorBridge.bringGameToFront(this, target)
                    if (via == null) {
                        log("gagal buka game: tidak ada launcher activity untuk $target")
                        return@Thread
                    }
                    log("game dibuka via $via — tunggu proses muncul...")
                    pid = InjectorBridge.waitForPid(target, 30)
                    if (pid == null) {
                        log("game tidak muncul setelah 30 detik — batal inject")
                        return@Thread
                    }
                } else {
                    log("game sudah jalan (pid $pid) — langsung inject")
                }
                runOnUiThread { binding.editPid.setText(pid.toString()) }
                log("found pid $pid for $target")

                // 2. Jangan inject ke proses yang baru setengah jalan: tunggu
                // libil2cpp.so ke-load dulu (payload juga nge-poll, tapi
                // nunggu di sini menghindari inject ke proses yang belum siap).
                log("tunggu libil2cpp.so ke-load...")
                if (!InjectorBridge.waitForLibIl2cpp(pid, 60)) {
                    log("libil2cpp.so tidak muncul setelah 60 detik — batal inject")
                    return@Thread
                }
                log("libil2cpp.so sudah ke-load — inject...")

                // 2b. Verify root is actually granted before inject: after
                // an uninstall/reinstall the root-manager grant is lost and
                // `su` can fail silently — v1.0.1 failed 4x with exit=1 and
                // empty output, payload never loaded.
                log("cek root...")
                val rootCheck = InjectorBridge.checkRoot()
                if (rootCheck.exitCode != 0 || !rootCheck.output.contains("uid=0")) {
                    log("root check: exit=${rootCheck.exitCode} output=${rootCheck.output}")
                    log("Root ditolak — buka aplikasi KernelSU/Magisk, izinkan root untuk Unity Controller Injector, lalu coba lagi")
                    return@Thread
                }
                log("root OK: ${rootCheck.output.trim()}")

                // 3. Inject seperti biasa.
                if (!InjectorBridge.resetPayloadStatus()) {
                    log("warning: payload status file could not be created — stage diagnostics will be missing")
                }
                log("jalankan (via su): ${InjectorBridge.injectCommand(this, pid)}")
                val result = InjectorBridge.inject(this, pid)
                log("injector exit=${result.exitCode} output=${result.output}")
                if (result.exitCode != 0) {
                    // Jangan lanjut ke auto-read status: payload tidak pernah
                    // ke-load, jadi stage log-nya pasti kosong. Diagnosis
                    // dari bukti yang ada (stderr + hasil cek root).
                    val cause = when {
                        !InjectorBridge.isRootAvailable() ->
                            "root ditolak (cek KernelSU/Magisk)"
                        result.output.contains("No such file", ignoreCase = true) ->
                            "binary/payload tidak ditemukan — ketuk Show paths dan cek"
                        else ->
                            "binary injector gagal — lihat output di atas"
                    }
                    log("inject GAGAL: $cause — status payload tidak dibaca (payload tidak pernah ke-load)")
                    return@Thread
                }
                // 4. Bawa game ke depan supaya logo floating langsung
                // kelihatan di dalam game.
                InjectorBridge.bringGameToFront(this, target)
                log("game dibawa ke depan — logo floating muncul di dalam game")
                // Payload init takes ~10-30s (libil2cpp wait + attach);
                // auto-read the stage log so the last reached stage is
                // visible without another tap.
                Thread.sleep(15000)
                showPayloadStatus("auto (15s after inject)")
            }.start()
        }

        binding.btnCheckStatus.setOnClickListener {
            Thread { showPayloadStatus("manual") }.start()
        }
    }

    private fun showPayloadStatus(source: String) {
        val result = InjectorBridge.readPayloadStatus()
        log("--- payload status [$source] ---")
        if (result.exitCode != 0 || result.output.isBlank()) {
            log("(empty — payload has not written any stage yet)")
        } else {
            log(result.output)
        }
    }

    private fun log(line: String) {
        runOnUiThread {
            binding.textLog.append(line + "\n")
        }
    }
}
