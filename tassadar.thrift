service TassadarServer {
    string version(),
    string get_ocr(1:binary image),
    string line_ocr(1:binary image),
    list<binary> cut_image(1:binary image, 2:i8 cut_type),
}
