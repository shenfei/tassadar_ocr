service TassadarServer {
    string version(),
    string get_ocr(1:binary image),
    string line_ocr(1:binary image),
}
