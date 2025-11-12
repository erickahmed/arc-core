/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

use arc_core::wrappers::point::Point;

#[test]
fn point_creation() {
    let p = Point::new(1.0, 2.0, 3.0).expect("Failed to create point");
    let coords = p.coordinates();
    assert_eq!(coords, (1.0, 2.0, 3.0));
}
