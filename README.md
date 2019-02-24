# tassadar_ocr
Tassadar is an OCR service based on [tesseract](https://github.com/tesseract-ocr/tesseract) and [thrift](https://github.com/apache/thrift).

## Usage

### API

Tassadar provides the following ocr APIs:

- `get_ocr(1:binary image)`: accept image binary data, return ocr text result.
- `line_ocr(1:bianry image)`: if you do image segmentation by yourself, you can just do ocr line by line, which gives you a more accurate result.
- `version()`: return the current version.

### Docker

The recommended way to use tassadar is through docker.
You can either choose a pre-build image from docker hub: `mathehacker/tassadar_ocr:latest`, or build a new one with [dockerfile](https://github.com/shenfei/tassadar_ocr/blob/master/tassadar.dockerfile).

Quick start:

```shell
docker pull mathehacker/tassadar_ocr:latest
docker tag mathehacker/tassadar_ocr:latest tassadar

# get the ocr result of $WORK_DIR/IMAGE_PATH
docker run -it -d --rm --name ocr -p 9090:9090 -v $WORK_DIR:/app tassadar /root/tassadar_ocr/tassadar_server
docker exec -it ocr python3 -m tassadar_client --input /app/IMAGE_PATH
```

### Build from source

You can also build tassadar from source code. Here are some tips.

First, make sure all the following dependencies are installed:

- tesseract >= 4.0
- thrift >= 0.11
- python3

Default tessdata language in tassadar is `chi_sim+eng`.
If you want to change it, please follow the [instruction in tessdata](https://github.com/tesseract-ocr/tessdata).

```shell
# server
git clone https://github.com/shenfei/tassadar_ocr.git
cd tassadar_ocr && make
./tassadar_server --port 9090

# client
pip3 install -e tassadar_ocr/python/
python3 -m tassadar_client -h
```

After installation, you can start a tassadar server and test ocr in python:

```python
from tassadar_client import TassadarClient

client = TassadarClient(host='localhost', port=9090)
with open(image_path, 'rb') as fin:
    image = fin.read()
print(client.get_ocr(image))
```

## Acknowledgment

The original tassadar project was developed during 2014 to 2016 in Uda Inc., a start-up which was closed in 2016.

I chose the name `tassadar` because it's similar to `tessearct` and all project names in Uda were picked from [StarCraft](https://starcraft.com/en-us/) at that moment.

Most of the outdated codes are removed, such as preprocessing via OpenCV, single character classification via [Caffee](https://github.com/BVLC/caffe), etc.
But I still thanks to those original contributors:

- [shenfei](https://github.com/shenfei)
- [Linusp](https://github.com/Linusp)
- [iwinux](https://github.com/iwinux)
- He Neng
