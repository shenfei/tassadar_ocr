#!/usr/bin/env python3
from setuptools import setup, find_packages

VERSION = '0.1.0'

REQS = [
    'thrift==0.11.0',
]

setup(
    name='tassadar_client',
    version=VERSION,
    url='https://github.com/shenfei/tassadar_ocr',
    description='Tassadar Python Thrift Client',
    packages=find_packages(),
    install_requires=REQS,
    entry_points={
        'console_scripts': [
            'tassadar_client = tassadar_client.__main__:main',
        ]
    },
)
