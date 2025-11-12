/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

mod ffi_point {
    //FFI

    use std::os::raw::c_double;

    #[repr(C)]
    pub(crate) struct PointShape {
        _private: [u8; 0],
    }

    unsafe extern "C" {
        fn make_point(x: c_double, y: c_double, z: c_double) -> *mut PointShape;
        fn coord_point(
            shape: *const PointShape,
            x: *mut c_double,
            y: *mut c_double,
            z: *mut c_double,
        );
        fn delete_point(shape: *mut PointShape);
    }

    // Safe wrapper

    pub struct Point {
        ptr: *mut PointShape,
    }

    impl Point {
        pub fn new(x: f64, y: f64, z: f64) -> Result<Self, &'static str> {
            unsafe {
                let ptr = make_point(x, y, z);
                if ptr.is_null() {
                    Err("Error: null pointer returned")
                } else {
                    Ok(Point { ptr })
                }
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

        pub fn delete(mut self) -> Result<(), &'static str> {
            unsafe {
                if self.ptr.is_null() {
                    return Err("Error: Attempted to delete null pointer");
                }
                delete_point(self.ptr);

                self.ptr = std::ptr::null_mut();
                std::mem::forget(self);
            }
            Ok(())
        }
    }

    impl Drop for Point {
        fn drop(&mut self) {
            unsafe {
                delete_point(self.ptr);
            }
        }
    }

    // TODO: check if i can mark as thread-safe

    impl std::fmt::Debug for Point {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            let (x, y, z) = self.coordinates();
            write!(f, "Point({}, {}, {})", x, y, z)
        }
    }
}

pub use ffi_point::Point;

// Basic tests
#[cfg(test)]
mod tests {
    use super::*;
    use std::os::raw::c_double;

    #[test]
    fn point_debug_precision() {
        let p = Point::new(1.123456789, 2.987654321, -3.555555555).expect("Failed to create point");
        let debug_str = format!("{:?}", p);

        let coords = p.coordinates();
        assert!(debug_str.contains(&coords.0.to_string()));
        assert!(debug_str.contains(&coords.1.to_string()));
        assert!(debug_str.contains(&coords.2.to_string()));
    }

    #[test]
    fn point_null_pointer_safety() {
        let result = Point::new(1.0, 2.0, 3.0);
        assert!(result.is_ok());

        let p = result.unwrap();
        let coords = p.coordinates();

        assert_eq!(coords, (1.0, 2.0, 3.0));
    }

    #[test]
    fn point_drop_impl_safety() {
        {
            let p = Point::new(1.0, 2.0, 3.0).expect("Failed to create point");
            // p goes out of scope here and should be dropped
        }
        let p2 = Point::new(4.0, 5.0, 6.0).expect("Failed to create point after drop");
        assert_eq!(p2.coordinates(), (4.0, 5.0, 6.0));
    }

    #[test]
    fn point_ffi_repr_c() {
        use std::mem;

        // PointShape should be repr(C) and zero-sized in Rust
        assert_eq!(mem::size_of::<ffi_point::PointShape>(), 0);

        // c_double should match f64
        assert_eq!(mem::size_of::<c_double>(), mem::size_of::<f64>());
        assert_eq!(mem::align_of::<c_double>(), mem::align_of::<f64>());
    }

    #[test]
    fn point_coordinates_after_multiple_uses() {
        let p = Point::new(1.1, 2.2, 3.3).expect("Failed to create point");

        let _coords1 = p.coordinates();
        let _debug1 = format!("{:?}", p);
        let _coords2 = p.coordinates();
        let _debug2 = format!("{:?}", p);
        let final_coords = p.coordinates();

        assert_eq!(final_coords, (1.1, 2.2, 3.3));
    }
}
