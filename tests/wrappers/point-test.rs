#[test]
mod tests {
    use arc_core::ffi::point::Point;

    fn point_creation() {
        let p = Point::new(1.0, 2.0, 3.0);
        let coords = p.coordinates();
        assert_eq!(coords, (1.0, 2.0, 3.0));
    }
}
