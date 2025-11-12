/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

fn main() {
    cc::Build::new()
        .cpp(true)
        .file("src/occt/point.cpp")
        .include("src/occt/include")
        .compile("point");

    println!("cargo:rerun-if-changed=src/occt/point.cpp");
    println!("cargo:rerun-if-changed=src/occt/include/point.hpp");
}
