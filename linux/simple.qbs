import qbs;

Product {
    name: "simple"
    type: [ "dynamiclibrary" ]
    consoleApplication: false

    Depends { name: "cpp" }

    cpp.commonCompilerFlags: [
        "-Wno-unused-parameter",
        "-Wno-unused-function",
        "-Wno-empty-body",
        "-Wno-sequence-point"
    ]

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.includePaths: outer.concat([
            "/usr/include/",
            "../interop/",
            "../"
        ])
        cpp.visibility: "hidden"
        cpp.linkerFlags: base.concat([
            "-Wl,--retain-symbols-file=" + sourceDirectory + "/simple.def"
        ])
    }

    Group {
        name: "exports"
        files: [
            "simple.def"
        ]
    }

    Group {
        name: "sources"
        prefix: "../"
        files: [
            "simpleinterop.c",
            "simple.c"
        ]
    }

    Group {
        name: "headers"
        prefix: "../"
        files: [
            "simple.h"
        ]
    }

    Group {
        name: "interop"
        prefix: "../interop/"
        files: [
            "dictionaryi.h",
            "interoplib.h",
            "interoplib.c",
            "interopstub.h",
            "workflowi.h",
            "taski.h",
        ]
    }

    Group {
        name: "jansson"
        prefix: "../jansson/"
        files: [
            "dump.c",
            "error.c",
            "hashtable.h",
            "hashtable.c",
            "hashtable_seed.c",
            "jansson.h",
            "jansson_config.h",
            "jansson_private.h",
            "load.c",
            "lookup3.h",
            "memory.c",
            "pack_unpack.c",
            "strbuffer.h",
            "strbuffer.c",
            "strconv.c",
            "utf.h",
            "utf.c",
            "value.c"
        ]
    }

    Group {
        // Copy produced library to install root
        fileTagsFilter: "dynamiclibrary"
        qbs.install: true
    }
}
