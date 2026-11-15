// Staged diagnostics for the injected payload.
//
// The payload runs inside the game process; when the game crashes there is
// no logcat available to the user, so the init thread writes one line per
// stage (with a unix timestamp) to this file, flushed after every line, so
// the last stage reached survives the crash and pinpoints where init died.
//
// The game UID cannot create files under /data/local/tmp, so the injector
// app (root) pre-creates the file with mode 666 before every inject; if
// the file cannot be opened, writes fail silently and nothing else breaks.
#ifndef UC_STATUS_LOG_H
#define UC_STATUS_LOG_H

#include <cstdio>
#include <ctime>

namespace StatusLog {
    static const char *kPath = "/data/local/tmp/unityinjector_status.txt";

    static inline void write(const char *stage) {
        FILE *f = fopen(kPath, "a");
        if (!f) return;
        fprintf(f, "%lld %s\n", (long long)time(NULL), stage);
        fflush(f);
        fclose(f);
    }

    static inline void writeHex(const char *key, uintptr_t value) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s=0x%llx", key, (unsigned long long)value);
        write(buf);
    }

    static inline void writeInt(const char *key, int value) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s=%d", key, value);
        write(buf);
    }
}

#endif
