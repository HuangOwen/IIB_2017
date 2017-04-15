
# Tips
1. 坐标体系中的零点坐标为图片的左上角，X轴为图像矩形的上面那条水平线；Y轴为图像矩形左边的那条垂直线。该坐标体系在诸如结构体Mat,Rect,Point中都是适用的
2. OpenCV自適應閾值二值化一樣是用threshold()函式，使用方式也一樣，只是最後一個參數增加CV_THRESH_OTSU，**目前otsu只能使用在8位元圖。** 
3. 

# Questions
1. What's the difference between `IplImage` and `Mat`? *Answer:* `Mat` is new in openCV 2.4 and is recommended.
2. How to convert a `Mat` to `OTSU` ? *Answer:* `src.converTo` and `cvtColor`
3. 
