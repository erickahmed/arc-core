/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

use arc_core::ffi::point::Point;

// Safe wrapper tests
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn creation_and_query() {
        let p = Point::new(1.0, 2.0, -3.0).expect("Safe creation failed");
        assert_eq!(p.coordinates(), (1.0, 2.0, -3.0));
    }

    #[test]
    fn manual_deletion() {
        let p = Point::new(1.0, 2.0, 3.0).expect("Creation failed");
        p.delete().expect("Manual deletion failed");
    }

    #[test]
    fn automatic_cleanup() {
        let coords = {
            let p = Point::new(7.0, 8.0, 9.0).expect("Creation failed");
            p.coordinates()
        };
        assert_eq!(coords, (7.0, 8.0, 9.0));
    }

    #[test]
    fn multiple_instances() {
        let p1 = Point::new(1.0, 1.0, 1.0).expect("Creation failed");
        let p2 = Point::new(2.0, 2.0, 2.0).expect("Creation failed");

        assert_eq!(p1.coordinates(), (1.0, 1.0, 1.0));
        assert_eq!(p2.coordinates(), (2.0, 2.0, 2.0));
    }

    #[test]
    fn debug_format() {
        let p = Point::new(1.1, 2.2, 3.3).expect("Creation failed");
        let debug_output = format!("{:?}", p);

        assert!(debug_output.starts_with("Point("));
        assert!(debug_output.contains("1.1"));
        assert!(debug_output.contains("2.2"));
        assert!(debug_output.contains("3.3"));
    }

    #[test]
    fn boundary_values() {
        let cases = [(0.0, 0.0, 0.0), (f64::MAX, f64::MIN, 0.0)];

        for (x, y, z) in cases {
            let p = Point::new(x, y, z).expect("Creation with boundary values failed");
            assert_eq!(p.coordinates(), (x, y, z));
        }
    }
}
