package com.hozinking.unityinjector

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.hozinking.unityinjector.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

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
                    runOnUiThread { binding.editPid.setText(pid.toString()) }
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
                val result = InjectorBridge.inject(this, pid)
                log("injector exit=${result.exitCode} output=${result.output}")
            }.start()
        }
    }

    private fun log(line: String) {
        runOnUiThread {
            binding.textLog.append(line + "\n")
        }
    }
}
