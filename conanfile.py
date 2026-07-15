from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeDeps, CMakeToolchain
from conan.tools.files import copy, collect_libs
import os


class FastQueueConan(ConanFile):
    name = "fastqueue"
    version = "0.1.0"
    license = "MIT"
    author = "Muhammad Fiaz"
    url = "https://github.com/muhammad-fiaz/FastQueue"
    homepage = "https://github.com/muhammad-fiaz/FastQueue"
    description = "Production-ready, high-performance C23 job system and thread pool library"
    topics = ("thread-pool", "job-system", "scheduler", "concurrency", "c23")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    exports_sources = "CMakeLists.txt", "src/*", "include/*"
    no_copy_source = True

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["FQ_BUILD_TESTS"] = False
        tc.variables["FQ_BUILD_EXAMPLES"] = False
        tc.variables["FQ_BUILD_BENCHMARKS"] = False
        tc.variables["FQ_INSTALL"] = True
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        cmake = CMake(self)
        cmake.install()

    def package_id(self):
        self.info.clear()

    def package_info(self):
        self.cpp_info.libs = collect_libs(self)
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.set_property("cmake_file_name", "FastQueue")
        self.cpp_info.set_property("cmake_target_name", "FastQueue::fastqueue")
        if self.settings.os in ("Linux", "FreeBSD"):
            self.cpp_info.systemlibs = ["pthread"]
        elif self.settings.os == "Windows":
            self.cpp_info.systemlibs = ["WinMM"]
