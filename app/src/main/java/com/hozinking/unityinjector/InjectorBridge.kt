package com.hozinking.unityinjector

import android.content.Context
import android.content.Intent
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

    /** Poll pidof until the process appears (or the timeout runs out). */
    fun waitForPid(processName: String, timeoutSec: Int): Int? {
        val deadline = System.currentTimeMillis() + timeoutSec * 1000L
        while (System.currentTimeMillis() < deadline) {
            val pid = findPid(processName)
            if (pid != null) return pid
            Thread.sleep(1000)
        }
        return findPid(processName)
    }

    /** True once the game has actually loaded its Unity runtime. */
    fun isLibIl2cppLoaded(pid: Int): Boolean {
        val r = RootShell.exec("grep -q libil2cpp.so /proc/$pid/maps")
        return r.exitCode == 0
    }

    /** Poll /proc/<pid>/maps until libil2cpp.so shows up (or timeout). */
    fun waitForLibIl2cpp(pid: Int, timeoutSec: Int): Boolean {
        val deadline = System.currentTimeMillis() + timeoutSec * 1000L
        while (System.currentTimeMillis() < deadline) {
            if (isLibIl2cppLoaded(pid)) return true
            Thread.sleep(1000)
        }
        return isLibIl2cppLoaded(pid)
    }

    /**
     * Launch the game if it is not running, or bring it to the foreground
     * if it already is. Returns a short description of the path used, or
     * null when the package has no launchable activity at all.
     */
    fun bringGameToFront(context: Context, packageName: String): String? {
        val pm = context.packageManager
        val intent = pm.getLaunchIntentForPackage(packageName)
        if (intent != null) {
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
            return try {
                context.startActivity(intent)
                "launch intent (PackageManager)"
            } catch (_: Exception) {
                null
            }
        }
        // Fallback: resolve the launcher activity and start it via root `am`.
        val launcher = Intent(Intent.ACTION_MAIN)
            .addCategory(Intent.CATEGORY_LAUNCHER)
            .setPackage(packageName)
        val activity = pm.queryIntentActivities(launcher, 0).firstOrNull()?.activityInfo
            ?: return null
        val component = "${activity.packageName}/${activity.name}"
        val r = RootShell.exec("am start -n $component")
        return if (r.exitCode == 0) "am start -n $component" else null
    }

    fun inject(context: Context, pid: Int): RootShell.Result {
        ensureExecutable(context)
        return RootShell.exec("${injectorPath(context)} $pid ${payloadPath(context)}")
    }
}
