set("version", "0.1.0")

option("fastqueue_shared", "Build as shared library", false)
option("fastqueue_tests", "Build tests", false)
option("fastqueue_examples", "Build examples", false)
option("fastqueue_benchmarks", "Build benchmarks", false)

target("fastqueue")
    set_kind("$(kind)")
    set_languages("c23")
    set_version("0.1.0")

    add_files("src/**/*.c")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(fastqueue/**)")

    if is_plat("windows") then
        add_links("WinMM")
    end

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    end

    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
    end

    if has_config("shared") or get_config("fastqueue_shared") then
        set_kind("shared")
    else
        set_kind("static")
    end

    if get_config("fastqueue_tests") then
        target("fq_tests")
            set_kind("binary")
            set_languages("c23")
            add_files("tests/**/*.c")
            add_deps("fastqueue")
            set_targetdir("build/tests")
    end

    if get_config("fastqueue_examples") then
        for _, file in ipairs(os.files("examples/*.c")) do
            local name = path.basename(file)
            target("fq_example_" .. name)
                set_kind("binary")
                set_languages("c23")
                add_files(file)
                add_deps("fastqueue")
                set_targetdir("build/examples")
        end
    end

    if get_config("fastqueue_benchmarks") then
        target("fq_benchmark")
            set_kind("binary")
            set_languages("c23")
            add_files("benchmarks/**/*.c")
            add_deps("fastqueue")
            set_targetdir("build/benchmarks")
    end
