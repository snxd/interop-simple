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
        cpp.linkerFlags: base.concat([
            "--version-script=" + sourceDirectory + "/simple.map",
            "--gc-sections"
        ])
        cpp.separateDebugInformation: true
    }

    Group {
        name: "exports"
        files: [
            "simple.map"
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
        // Copy produced library to install root
        fileTagsFilter: "dynamiclibrary"
        qbs.install: true
    }
    Group {
        // Copy debug symbols to install root
        fileTagsFilter: "debuginfo_dll"
        qbs.install: true
    }
}
