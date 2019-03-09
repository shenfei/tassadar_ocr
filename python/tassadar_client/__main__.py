import argparse

from .tassadar_client import TassadarClient


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int, default=9090)
    parser.add_argument('--host', default='localhost')
    parser.add_argument('--version', action='store_true')
    parser.add_argument('--line', action='store_true')
    parser.add_argument('--cut', action='store_true')
    parser.add_argument('--cut_type', type=int, default=4)
    parser.add_argument('--input')
    parser.add_argument('--output', default='./')
    args = parser.parse_args()

    client = TassadarClient(host=args.host, port=args.port)

    if args.version:
        print(client.version())

    elif args.cut:
        with open(args.input, 'rb') as fin:
            image = fin.read()
        sub_images = client.cut_image(image)
        for i, img in enumerate(sub_images):
            out_path = os.path.join(args.output, f'{i}.png')
            with open(out_path, 'wb') as fout:
                fout.write(img)

    else:
        with open(args.input, 'rb') as fin:
            image = fin.read()
        if args.line:
            print(client.line_ocr(image))
        else:
            print(client.get_ocr(image))


if __name__ == "__main__":
    main()
