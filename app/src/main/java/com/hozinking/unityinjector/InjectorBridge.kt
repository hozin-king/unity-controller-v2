package com.hozinking.unityinjector

import android.content.Context
import java.io.File

/**
 * libinjector.so and libpayload.so both land in applicationInfo.nativeLibraryDir
 * because the Android Gradle Plugin packages them there like any other native
 * library (see CMakeLists.txt and README "Packaging check" for why
 * libinjector.so, which is really a standalone executable, ends up there too).
 */
object InjectorBridge {

    private const val INJECTOR_LIB = "libinjector.so"
    private const val PAYLOAD_LIB = "libHarshitController.so"

    /** Stage log written by the payload thread inside the game process. */
    const val STATUS_PATH = "/data/local/tmp/unityinjector_status.txt"

    /**
     * The game process runs as its own UID and cannot create files under
     * /data/local/tmp, so (re)create the status file as root before every
     * inject — otherwise the payload's stage writes would silently fail.
     */
    fun resetPayloadStatus(): Boolean {
        val r = RootShell.exec("rm -f $STATUS_PATH; touch $STATUS_PATH; chmod 666 $STATUS_PATH")
        return r.exitCode == 0
    }

    fun readPayloadStatus(): RootShell.Result = RootShell.exec("cat $STATUS_PATH")

    fun injectorPath(context: Context): String =
        File(context.applicationInfo.nativeLibraryDir, INJECTOR_LIB).absolutePath

    fun payloadPath(context: Context): String =
        File(context.applicationInfo.nativeLibraryDir, PAYLOAD_LIB).absolutePath

    /**
     * Defensive chmod in case the native lib directory wasn't extracted with
     * the executable bit set for this file. Cheap, safe to call every time.
     */
    fun ensureExecutable(context: Context): Boolean {
        val result = RootShell.exec("chmod 755 '${injectorPath(context)}'")
        return result.exitCode == 0
    }

    fun findPid(processName: String): Int? {
        val result = RootShell.exec("pidof $processName")
        val pid = result.output.trim().split(Regex("\\s+")).firstOrNull()?.toIntOrNull()
        return if (result.exitCode == 0) pid else null
    }

    fun inject(context: Context, pid: Int): RootShell.Result {
        ensureExecutable(context)
        return RootShell.exec("${injectorPath(context)} $pid ${payloadPath(context)}")
    }
}
