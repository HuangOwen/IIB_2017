
# Tips
1. 坐标体系中的零点坐标为图片的左上角，X轴为图像矩形的上面那条水平线；Y轴为图像矩形左边的那条垂直线。该坐标体系在诸如结构体Mat,Rect,Point中都是适用的
2. OpenCV自適應閾值二值化一樣是用threshold()函式，使用方式也一樣，只是最後一個參數增加CV_THRESH_OTSU，**目前otsu只能使用在8位元圖。** 
3. `Vec4i` 转化为 `Vec4d`(double) 更精确，否则计算交点会出现极大误差

# Questions
1. What's the difference between `IplImage` and `Mat`? *Answer:* `Mat` is new in openCV 2.4 and is recommended.
2. How to convert a `Mat` to `OTSU` ? *Answer:* `src.converTo` and `cvtColor`


## HoughLines
`void HoughLines(InputArray image, OutputArray lines, double rho, double theta, int threshold, double srn=0, double stn=0)`

image：輸入圖，8位元單通道二值化圖。
lines：將所有線的資料存在vector< Vec2f >，Vec2f為每個線的資料，分別有ρ、θ這兩個參數，ρ表示和左上角(0,0)的距離，θ是線的旋轉角度，單位弧度，垂直線的θ為0，水平線的θ為π/2。
rho：距離解析度，越小表示定位要求越準確，但也較易造成應該是同條線的點判為不同線。
theta：角度解析度，越小表示角度要求越準確，但也較易造成應該是同條線的點判為不同線。
threshold：累積個數閾值，超過此值的線才會存在lines這個容器內。
srn：可有可無的距離除數。
stn：可有可無的角度除數。

## Circle
`void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)`

img：輸入圖，圓會畫在上面。
center：圓心。
radius：圓半徑。
color：圓形的顏色。
thickness：圓形的邊線寬度，輸入負值或CV_FILLED代表填滿圓形。
lineType：通道型態，可輸入8、4、CV_AA： 8->8通道連結。 4->4通道連結。 CV_AA->消除鋸齒(antialiased line)，消除顯示器畫面線邊緣的凹凸鋸齒。

## HoughCircle
`void HoughCircles(InputArray image, OutputArray circles, int method, double dp, double minDist, double param1=100, doubleparam2=100, int minRadius=0, int maxRadius=0)`

image：輸入圖，8位元單通道圖。
circles：以vector< Vec3f >記錄所有圓的資訊，每個Vec3f紀錄一個圓的資訊，包含3個浮點數資料，分別表示x、y、radius。
method：偵測圓的方法，目前只能使用CV_HOUGH_GRADIENT。
dp：偵測解析度倒數比例，假設dp=1，偵測圖和輸入圖尺寸相同，假設dp=2，偵測圖長和寬皆為輸入圖的一半。
minDist：圓彼此間的最短距離，太小的話可能會把鄰近的幾個圓視為一個，太大的話可能會錯過某些圓。
param1：圓偵測內部會呼叫Canny()尋找邊界，param1就是Canny()的高閾值，低閾值自動設為此值的一半。
param2：計數閾值，超過此值的圓才會存入circles。
minRadius：最小的圓半徑。
maxRadius：最大的圓半徑。
