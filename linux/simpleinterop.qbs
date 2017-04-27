import qbs;

Product {
    name: "simpleinterop"
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
            "../jansson/",
            "../interop/"
        ])
        cpp.visibility: "hidden"
        cpp.linkerFlags: base.concat([
            "-Wl,--retain-symbols-file=" + sourceDirectory + "/simpleinterop.def"
        ])
    }

    Group {
        name: "exports"
        files: [
            "simpleinterop.def"
        ]
    }

    Group {
        name: "sources"
        prefix: "../"
        files: [
            "simpleinterop.c",
            "simpleobject.c"
        ]
    }

    Group {
        name: "headers"
        prefix: "../"
        files: [
            "simpleobject.h"
        ]
    }

    Group {
        name: "interop"
        prefix: "../interop/"
        files: [
            "interoplib.h",
            "interopstub.h"
        ]
    }

    Group {
        name: "jansson"
        prefix: "../jansson/"
        files: [
            "hashtable.h",
            "hashtable.c",
            "jansson.h",
            "jansson_private.h",
            "strbuffer.h",
            "strbuffer.c",
            "utf.h",
            "utf.c",
            "util.h",
            "dump.c",
            "load.c",
            "value.c"
        ]
    }

    Group {
        // Copy produced library to install root
        fileTagsFilter: "dynamiclibrary"
        qbs.install: true
    }
}
