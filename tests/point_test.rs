/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

use arc_core::ffi::point::Point;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn point_creation() {
        let p = Point::new(1.0, 2.0, -3.0).expect("Failed to create point");
        let coords = p.coordinates();
        assert_eq!(coords, (1.0, 2.0, -3.0));
    }

    #[test]
    fn point_query() {
        let p = Point::new(5.5, 3.15, -0.001).expect("Failed to create point");
        let coords = p.coordinates();
        assert_eq!(coords, (5.5, 3.15, -0.001));
    }

    #[test]
    fn point_deletion() {
        let p = Point::new(1.0, 2.0, 3.0).expect("Failed to create point");
        let result = p.delete();
        assert!(result.is_ok(), "Deletion should succeed for valid point");
    }

    #[test]
    fn point_creation_extreme_values() {
        let cases = [
            (f64::MAX, f64::MIN, 0.0),
            (0.0, 0.0, 0.0),
            (-f64::MAX, -f64::MIN, f64::EPSILON),
        ];

        for (x, y, z) in cases {
            let p = Point::new(x, y, z).expect("Failed to create point with extreme values");
            let coords = p.coordinates();
            assert_eq!(coords, (x, y, z));
            p.delete().expect("Failed to delete point");
        }
    }

    #[test]
    fn point_coordinates_immutability() {
        let p = Point::new(1.5, 2.5, 3.5).expect("Failed to create point");

        let coords1 = p.coordinates();
        let coords2 = p.coordinates();
        let coords3 = p.coordinates();

        assert_eq!(coords1, coords2);
        assert_eq!(coords2, coords3);
        assert_eq!(coords1, (1.5, 2.5, 3.5));
    }

    #[test]
    fn point_automatic_cleanup_on_drop() {
        let coords = {
            let p = Point::new(7.0, 8.0, 9.0).expect("Failed to create point");
            p.coordinates()
        };

        assert_eq!(coords, (7.0, 8.0, 9.0));

        // Verify we can still create points after automatic cleanup
        let p2 = Point::new(10.0, 11.0, 12.0).expect("Failed to create point after drop");
        assert_eq!(p2.coordinates(), (10.0, 11.0, 12.0));
    }

    #[test]
    fn point_multiple_instances_independent() {
        let p1 = Point::new(1.0, 1.0, 1.0).expect("Failed to create point 1");
        let p2 = Point::new(2.0, 2.0, 2.0).expect("Failed to create point 2");
        let p3 = Point::new(3.0, 3.0, 3.0).expect("Failed to create point 3");

        assert_eq!(p1.coordinates(), (1.0, 1.0, 1.0));
        assert_eq!(p2.coordinates(), (2.0, 2.0, 2.0));
        assert_eq!(p3.coordinates(), (3.0, 3.0, 3.0));

        p2.delete().expect("Failed to delete p2");
        p1.delete().expect("Failed to delete p1");
        p3.delete().expect("Failed to delete p3");
    }

    #[test]
    fn point_debug_format() {
        let p = Point::new(1.1, 2.2, 3.3).expect("Failed to create point");
        let debug_output = format!("{:?}", p);

        assert!(debug_output.contains("Point"));
        assert!(debug_output.contains("1.1"));
        assert!(debug_output.contains("2.2"));
        assert!(debug_output.contains("3.3"));
    }

    #[test]
    fn point_rapid_creation_deletion_cycle_stress_test() {
        for i in 0..100 {
            let p = Point::new(i as f64, i as f64, i as f64)
                .expect(&format!("Failed to create point in iteration {}", i));
            assert_eq!(p.coordinates(), (i as f64, i as f64, i as f64));
            p.delete()
                .expect(&format!("Failed to delete point in iteration {}", i));
        }
    }
}
