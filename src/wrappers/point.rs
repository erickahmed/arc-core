mod ffi_point {
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
