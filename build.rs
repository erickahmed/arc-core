/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

fn main() {
    println!("cargo:rustc-link-search=/usr/local/lib");
    println!("cargo:rustc-link-search=/app/build");

    println!("cargo:rustc-link-lib=occt");

    println!("cargo:rerun-if-changed=src/wrappers/");
}
