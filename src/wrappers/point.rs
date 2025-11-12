/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

use std::os::raw::c_double;

// FFI bindings

#[repr(C)]
pub struct PointShape {
    _private: [u8; 0],
}

extern "C" {
    pub fn make_point(x: c_double, y: c_double, z: c_double) -> *mut PointShape;
    pub fn coord_point(
        shape: *const PointShape,
        x: *mut c_double,
        y: *mut c_double,
        z: *mut c_double,
    );
    pub fn delete_point(shape: *mut PointShape);
}

    extern "C" {
        fn make_point(x: c_double, y: c_double, z: c_double) -> *mut PointShape;
        fn coord_point(
            shape: *const PointShape,
            x: *mut c_double,
            y: *mut c_double,
            z: *mut c_double,
        );
        fn delete_point(shape: *mut PointShape);
    }

pub struct Point {
    ptr: *mut PointShape,
}

impl Point {
    pub fn new(x: f64, y: f64, z: f64) -> Self {
        unsafe {
            let ptr = make_point(x, y, z);
            if ptr.is_null() {
                panic!("Error: null pointer");
            }
            Self { ptr }
        }
    }

    pub fn coordinates(&self) -> (f64, f64, f64) {
        unsafe {
            let mut x = 0.0;
            let mut y = 0.0;
            let mut z = 0.0;

            coord_point(self.ptr, &mut x, &mut y, &mut z);
            (x, y, z)
        }
    }
}

impl Drop for Point {
    fn drop(&mut self) {
        unsafe {
            delete_point(self.ptr);
        }
    }
}

impl Clone for Point {
    fn clone(&self) -> Self {
        let (x, y, z) = self.coordinates();
        Self::new(x, y, z)
    }
}

unsafe impl Send for Point {}
unsafe impl Sync for Point {}
