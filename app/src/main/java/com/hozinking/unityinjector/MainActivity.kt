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

        // Inject stays disabled until a PID is known: tapping Inject with
        // no PID was the exit=1 mistake in the v1.0 POCO test.
        binding.btnInject.isEnabled = false
        binding.editPid.addTextChangedListener(object : TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {}
            override fun afterTextChanged(s: Editable?) {
                binding.btnInject.isEnabled = s.toString().trim().toIntOrNull() != null
            }
        })

        binding.btnCheckRoot.setOnClickListener {
            Thread {
                val available = RootShell.isRootAvailable()
                log(if (available) "root: available" else "root: NOT available")
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
            val pid = binding.editPid.text.toString().trim().toIntOrNull()
            if (pid == null) {
                log("set a valid pid first (use Find PID or type it in)")
                return@setOnClickListener
            }
            Thread {
                if (!InjectorBridge.resetPayloadStatus()) {
                    log("warning: payload status file could not be created — stage diagnostics will be missing")
                }
                val result = InjectorBridge.inject(this, pid)
                log("injector exit=${result.exitCode} output=${result.output}")
                if (result.exitCode == 0) {
                    // Payload init takes ~10-30s (libil2cpp wait + attach);
                    // auto-read the stage log so the last reached stage is
                    // visible without another tap.
                    Thread.sleep(15000)
                    showPayloadStatus("auto (15s after inject)")
                }
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
