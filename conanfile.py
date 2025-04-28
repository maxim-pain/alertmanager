from conan import ConanFile
from conan.tools.files import copy, collect_libs 
import os
from conan.tools.build import cross_building, check_min_cppstd, default_cppstd
from conan.tools.cmake import cmake_layout

from conan.tools.cmake import CMakeToolchain


class HelloWorldRecipe(ConanFile):
    name = "helloworld"
    package_type = "library"
    homepage = "https://github.com/maxim-pain/max-Alert-Manager.git"
    description = "Alert Manager for medical device notifications."
    topics = ("alert-manager", "medical-device", "example")
    settings = "os", "compiler", "build_type", "arch"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": True,
    }

    def requirements(self):
        self.requires("protobuf/3.21.12")
        self.test_requires("gtest/1.16.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.31.6")
        self.tool_requires("protobuf/3.21.12")
