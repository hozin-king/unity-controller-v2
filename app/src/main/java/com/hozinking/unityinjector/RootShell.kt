package com.hozinking.unityinjector

import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.InputStreamReader

/**
 * One persistent `su` shell, reused across calls instead of spawning a new
 * su process (and a new superuser grant prompt) per command. Not
 * thread-safe beyond the @Synchronized guards below: only call this from
 * one worker thread at a time.
 */
object RootShell {

    data class Result(val exitCode: Int, val output: String)

    private const val MARKER = "__ROOTINJECTOR_DONE__"

    @Volatile
    private var process: Process? = null
    private var stdin: DataOutputStream? = null
    private var stdout: BufferedReader? = null

    @Synchronized
    fun open(): Boolean {
        if (process != null) return true
        return try {
            val p = Runtime.getRuntime().exec("su")
            process = p
            stdin = DataOutputStream(p.outputStream)
            stdout = BufferedReader(InputStreamReader(p.inputStream))
            true
        } catch (e: Exception) {
            process = null
            false
        }
    }

    @Synchronized
    fun exec(command: String): Result {
        if (process == null && !open()) {
            return Result(-1, "root shell unavailable (no su, or user denied the grant prompt)")
        }
        val outBuilder = StringBuilder()
        return try {
            stdin!!.writeBytes("$command\n")
            stdin!!.writeBytes("echo $MARKER:\$?\n")
            stdin!!.flush()

            while (true) {
                val line = stdout!!.readLine() ?: break
                if (line.startsWith("$MARKER:")) {
                    val code = line.removePrefix("$MARKER:").trim().toIntOrNull() ?: -1
                    return Result(code, outBuilder.toString().trim())
                }
                outBuilder.append(line).append('\n')
            }
            Result(-1, outBuilder.toString().trim())
        } catch (e: Exception) {
            close()
            Result(-1, "root shell error: ${e.message}")
        }
    }

    @Synchronized
    fun close() {
        try {
            stdin?.writeBytes("exit\n")
            stdin?.flush()
        } catch (_: Exception) {
        }
        process?.destroy()
        process = null
        stdin = null
        stdout = null
    }

    fun isRootAvailable(): Boolean {
        val r = exec("id -u")
        return r.exitCode == 0 && r.output.trim() == "0"
    }
}
