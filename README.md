# vtkDataStreamers

A very quick attempt to compile and use some of the file series readers from ParaView. It is not configured correct for running CTest yet.

The test can be executed by specifying the `-T` option, e.g. for testing the image series reader, one can do

```
./vtkSpsIOImageCxxTests  TestImageFileSeriesReader -T ~/github/vtkDataStreamers/build
```

