
// 201713074DIPView.cpp : implementation of the CMy201713074DIPView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "201713074DIP.h"
#endif

#include "201713074DIPDoc.h"
#include "201713074DIPView.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy201713074DIPView

IMPLEMENT_DYNCREATE(CMy201713074DIPView, CView)

BEGIN_MESSAGE_MAP(CMy201713074DIPView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMGLOAD_BMP, &CMy201713074DIPView::OnImgloadBmp)
	ON_COMMAND(ID_IMGLOAD_JPG, &CMy201713074DIPView::OnImgloadJpg)
	ON_COMMAND(ID_CONVERSION_RGBTOHSI, &CMy201713074DIPView::OnConversionRgbtohsi)
	ON_COMMAND(ID_GREYLEVEL_NEGATIVE, &CMy201713074DIPView::OnGreylevelNegative)
	ON_COMMAND(ID_GREYLEVEL_GREYSCALE, &CMy201713074DIPView::OnGreylevelGreyscale)
	ON_COMMAND(ID_GREYLEVEL_THRESHOLDING, &CMy201713074DIPView::OnGreylevelThresholding)
	ON_COMMAND(ID_GREYLEVEL_POWERLAW, &CMy201713074DIPView::OnGreylevelPowerlaw)
	ON_COMMAND(ID_GREYLEVEL_HISTOGRAMEQUALIZATION, &CMy201713074DIPView::OnGreylevelHistogramequalization)
	ON_COMMAND(ID_FILTERING_GAUSSIAN, &CMy201713074DIPView::OnFilteringGaussian)
	ON_COMMAND(ID_FILTERING_MEDIAN, &CMy201713074DIPView::OnFilteringMedian)
	ON_COMMAND(ID_FILTERING_AVERAGE, &CMy201713074DIPView::OnFilteringAverage)
	ON_COMMAND(ID_FILTERING_COLORMEDIAN, &CMy201713074DIPView::OnFilteringColormedian)
	ON_COMMAND(ID_FILTERING_HIGHBOOST, &CMy201713074DIPView::OnFilteringHighboost)
	ON_COMMAND(ID_EDGE_PREWITT, &CMy201713074DIPView::OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CMy201713074DIPView::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_LOG, &CMy201713074DIPView::OnEdgeLog)
	ON_COMMAND(ID_VIDEO_AVI, &CMy201713074DIPView::OnVideoAvi)
	ON_COMMAND(ID_VIDEO_MP4, &CMy201713074DIPView::OnVideoMp4)
	ON_COMMAND(ID_VIDEO_LUCASKANADE, &CMy201713074DIPView::OnVideoLucaskanade)
END_MESSAGE_MAP()


//멤버변수
RGBQUAD** rgbBuffer; //이미지 저장할 변수
int imgHeight; //이미지 높이
int imgWidth; //이미지 너비
BITMAPINFOHEADER bmpInfo; //bmp 정보 헤더
BITMAPFILEHEADER bmpHeader; //bmp 파일 헤더
float** hueBuffer; //색조
float** satuBuffer; //채도
float** intenBuffer; //명도
int viewtype;
int histogramPrint[256][100];


// CMy201713074DIPView construction/destruction

CMy201713074DIPView::CMy201713074DIPView() //생성자
{
	// TODO: add construction code here
	rgbBuffer = nullptr; //초기화
	hueBuffer = nullptr;
	satuBuffer = nullptr;
	intenBuffer = nullptr;
	viewtype = -1;
}

CMy201713074DIPView::~CMy201713074DIPView() //소멸자
{
	if (rgbBuffer != nullptr) { //해제
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}
	if (hueBuffer != nullptr) { //해제
		for (int i = 0; i < imgHeight; i++)
			delete[] hueBuffer[i];
		delete[] hueBuffer;
	}
	if (satuBuffer != nullptr) { //해제
		for (int i = 0; i < imgHeight; i++)
			delete[] satuBuffer[i];
		delete[] satuBuffer;
	}
	if (intenBuffer != nullptr) { //해제
		for (int i = 0; i < imgHeight; i++)
			delete[] intenBuffer[i];
		delete[] intenBuffer;
	}
}

BOOL CMy201713074DIPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy201713074DIPView drawing

void CMy201713074DIPView::OnDraw(CDC* pDC)
{
	CMy201713074DIPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (rgbBuffer != NULL) {//이미지가 들어있는 경우 출력
		for (int i = 0; i < imgHeight; i++) {
			for (int j = 0; j < imgWidth; j++) {
				POINT p; //출력할 픽셀의 위치 지정
				p.x = j;
				p.y = i;
				//픽셀 위치 p에 rgb 값 출력
				pDC->SetPixel(p, RGB(rgbBuffer[i][j].rgbRed, rgbBuffer[i][j].rgbGreen, rgbBuffer[i][j].rgbBlue));
			}
		}
	}
	
	if (viewtype==2) {
		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 100; j++) {
				POINT p; //출력할 픽셀의 위치 지정
				p.x = i + imgWidth + 50;
				p.y = j;
				//픽셀 위치 p에 rgb 값 출력
				pDC->SetPixel(p, RGB(histogramPrint[i][j], histogramPrint[i][j], histogramPrint[i][j]));
			}
		}
	}
	
	// TODO: add draw code for native data here
}


// CMy201713074DIPView printing

BOOL CMy201713074DIPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy201713074DIPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy201713074DIPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMy201713074DIPView diagnostics

#ifdef _DEBUG
void CMy201713074DIPView::AssertValid() const
{
	CView::AssertValid();
}

void CMy201713074DIPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy201713074DIPDoc* CMy201713074DIPView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy201713074DIPDoc)));
	return (CMy201713074DIPDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy201713074DIPView message handlers


void CMy201713074DIPView::OnImgloadBmp() //이미지로드-bmp 메뉴
{
	// TODO: Add your command handler code here
	//1. bmp 파일 입력
	CFileDialog dlg(TRUE, ".bmp", NULL, NULL, "Bitmap File (*.bmp)|*bmp||");
	if (IDOK != dlg.DoModal())
		return;
	CString filename = dlg.GetPathName();
	if (rgbBuffer != NULL) { //할당되어 있을 경우 메모리 해제
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}

	//2. 파일 오픈, 영상 정보 획득
	CFile file;
	file.Open(filename, CFile::modeRead);
	file.Read(&bmpHeader, sizeof(BITMAPFILEHEADER));
	file.Read(&bmpInfo, sizeof(BITMAPINFOHEADER));
	imgWidth = bmpInfo.biWidth;
	imgHeight = bmpInfo.biHeight;

	//3. 이미지 저장할 버퍼[이미지높이*이미지너비] 할당
	rgbBuffer = new RGBQUAD*[imgHeight];
	for (int i = 0; i < imgHeight; i++)
		rgbBuffer[i] = new RGBQUAD[imgWidth];

	//4. 이미지 너비가 4의 배수인지 체크: 
	/*
	bmp 조건 가로는 4바이트씩이어야 함
	한 픽셀은 3바이트(rgb) -> 가로*3 이 4의 배수인지 알아야 함
	*/
	bool b4byte = false;//4바이트의 배수인지 표시
	int upbyte = 0;//4바이트의 배수에서 몇 바이트나 모자라는지
	if ((imgWidth * 3) % 4 == 0) { //4의 배수다!
		b4byte = true;
		upbyte = 0;
	}
	else { //4의 배수가 아니다!
		b4byte = false;
		upbyte = 4 - (imgWidth * 3) % 4;
	}

	//5. 파일에서 픽셀 데이터 읽어오기
	BYTE data[3];
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			file.Read(&data, 3);
			//이미지가 거꾸로 저장되어 있음->거꾸로 읽어옴
			rgbBuffer[imgHeight - i - 1][j].rgbBlue = data[0];
			rgbBuffer[imgHeight - i - 1][j].rgbGreen = data[1];
			rgbBuffer[imgHeight - i - 1][j].rgbRed = data[2];
		}
		if (b4byte == false) //가로가 4바이트의 배수가 아니면 (맞춰주려고) 쓰레기값을 읽음
			file.Read(&data, upbyte);
	}
	file.Close(); //파일 닫기
	Invalidate(TRUE); //화면 갱신
}


BYTE * LoadJpegFromOpenFile(FILE *fp, BITMAPINFOHEADER *pbh, UINT *pWidth, UINT *pHeight) {
	//파일로부터 jpg 정보 획득
	if (pWidth == NULL || pHeight == NULL)
		return NULL;
	BYTE *tmp = JpegFile::OpenFileToRGB(fp, pWidth, pHeight);
	if (!tmp)
		return NULL;
	JpegFile::BGRFromRGB(tmp, *pWidth, *pHeight);
	UINT dw;
	BYTE *pbuf = JpegFile::MakeDwordAlignedBuf(tmp, *pWidth, *pHeight, &dw);
	delete[] tmp;
	if (!pbuf)
		return NULL;
	JpegFile::VertFlipBuf(pbuf, dw, *pHeight);
	pbh->biSize = sizeof(BITMAPINFOHEADER);
	pbh->biWidth = *pWidth;
	pbh->biHeight = *pHeight;
	pbh->biPlanes = 1;
	pbh->biBitCount = 24;
	pbh->biCompression = BI_RGB;
	pbh->biSizeImage = 0;
	pbh->biXPelsPerMeter = 0;
	pbh->biYPelsPerMeter = 0;
	pbh->biClrUsed = 0;
	pbh->biClrImportant = 0;
	return pbuf;
}


void CMy201713074DIPView::OnImgloadJpg()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, ".jpg", NULL, NULL, "Jpeg File(*.jpg)|*.jpg||");
	if (IDOK != dlg.DoModal())
		return;
	CString filename = dlg.GetPathName();

	//rgb버퍼가 null이 아닐 경우 메모리 해제
	if (rgbBuffer != NULL) {
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}

	FILE *fp = fopen(filename, "rb");
	BITMAPINFOHEADER pbh;
	UINT w, h;
	BYTE *pbuf = LoadJpegFromOpenFile(fp, &pbh, &w, &h);
	imgWidth = (int)w;
	imgHeight = (int)h;

	//rgb버퍼에 이미지 사이즈만큼 메모리 할당
	rgbBuffer = new RGBQUAD*[imgHeight];
	for (int i = 0; i < imgHeight; i++)
		rgbBuffer[i] = new RGBQUAD[imgWidth];

	int dw = WIDTHBYTES(imgWidth * 24);
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			rgbBuffer[imgHeight - i - 1][j].rgbBlue = pbuf[i*dw + j * 3 + 0];
			rgbBuffer[imgHeight - i - 1][j].rgbGreen = pbuf[i*dw + j * 3 + 1];
			rgbBuffer[imgHeight - i - 1][j].rgbRed = pbuf[i*dw + j * 3 + 2];
		}
	}

	delete[] pbuf;
	fclose(fp);
	Invalidate(TRUE);
}


void CMy201713074DIPView::OnConversionRgbtohsi()
{
	// TODO: Add your command handler code here
	//rgb 버퍼에 이미지 있는지 확인
	if (rgbBuffer == NULL)
		OnImgloadJpg();

	//변수 메모리 할당
	hueBuffer = new float*[imgHeight];
	satuBuffer = new float*[imgHeight];
	intenBuffer = new float*[imgHeight];

	for (int i = 0; i < imgHeight; i++) {
		hueBuffer[i] = new float[imgWidth];
		satuBuffer[i] = new float[imgWidth];
		intenBuffer[i] = new float[imgWidth];
	}

	//rgb->hsi
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			float r = rgbBuffer[i][j].rgbRed;
			float g = rgbBuffer[i][j].rgbGreen;
			float b = rgbBuffer[i][j].rgbBlue;

			intenBuffer[i][j] = (r + g + b) / (float)(3 * 255);

			float total = r + g + b;
			r = r / total;
			g = g / total;
			b = b / total;
			satuBuffer[i][j] = 1 - 3 * (r > g ? (g > b ? b : g) : (r > b ? b : r));
			if ((r == g) && (g = b)) {
				hueBuffer[i][j] = 0;
				satuBuffer[i][j] = 0;
			}
			else {
				total = (0.5*(r - g + r - b) / sqrt((r - g)*(r - g) + (r - b)*(g - b)));
				hueBuffer[i][j] = acos((double)total);
				if (b > g) {
					hueBuffer[i][j] = 6.28 - hueBuffer[i][j];
				}
			}
		}
	}

	//출력 값 범위 정규화: [0,255]
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			hueBuffer[i][j] * 255 / (3.14 * 2);
			satuBuffer[i][j] = satuBuffer[i][j] * 255;
			intenBuffer[i][j] = intenBuffer[i][j] * 255;
		}
	}

	//출력
	Invalidate(FALSE);
	//Invalidate(TRUE);
}


void CMy201713074DIPView::OnGreylevelGreyscale()
{
	// TODO: Add your command handler code here
	if(intenBuffer == NULL)
		OnConversionRgbtohsi();

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			rgbBuffer[i][j].rgbRed = intenBuffer[i][j];
			rgbBuffer[i][j].rgbGreen = intenBuffer[i][j];
			rgbBuffer[i][j].rgbBlue = intenBuffer[i][j];			
		}
	}

	Invalidate(TRUE);
}


void CMy201713074DIPView::OnGreylevelThresholding()
{
	// TODO: Add your command handler code here
	OnConversionRgbtohsi();
	
	/*
	OnGreylevelGreyscale();
	float criinten = 127.0;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			if (intenBuffer[i][j] >= criinten) {
				rgbBuffer[i][j].rgbRed = 0.0;
				rgbBuffer[i][j].rgbGreen = 0.0;
				rgbBuffer[i][j].rgbBlue = 0.0;
			}
			else {
				rgbBuffer[i][j].rgbRed = 255.0;
				rgbBuffer[i][j].rgbGreen = 255.0;
				rgbBuffer[i][j].rgbBlue = 255.0;
			}
		}
	}
	*/

	///*
	//색조(hue)범위이내면 검정(0,0,0), 아니면 하양(255,255,255)
	float minhue = 0.24;
	float maxhue = 1.0;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			if ((hueBuffer[i][j] >= minhue) && (hueBuffer[i][j] <= maxhue)) {
				rgbBuffer[i][j].rgbRed = 0.0;
				rgbBuffer[i][j].rgbGreen = 0.0;
				rgbBuffer[i][j].rgbBlue = 0.0;
			}
			else {
				rgbBuffer[i][j].rgbRed = 255.0;
				rgbBuffer[i][j].rgbGreen = 255.0;
				rgbBuffer[i][j].rgbBlue = 255.0;
			}			
		}
	}
	//*/

	Invalidate(TRUE);
}


float** Negative(float** hsi, int w, int h) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			hsi[i][j] = 255 - hsi[i][j];
		}
	}
	return hsi;
}


void CMy201713074DIPView::OnGreylevelNegative()
{
	// TODO: Add your command handler code here
	//hsi 버퍼에 이미지 있는지 확인
	OnConversionRgbtohsi();

	intenBuffer = Negative(intenBuffer, imgWidth, imgHeight);

	OnGreylevelGreyscale();

	//Invalidate(TRUE);
}


void CMy201713074DIPView::OnGreylevelPowerlaw()
{
	// TODO: Add your command handler code here
	//hsi 변환, 정규화
	OnConversionRgbtohsi();
	
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			intenBuffer[i][j] = intenBuffer[i][j] / 255;
		}
	}

	//y=cx^r
	//c=1

	double r = 2.5;

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			intenBuffer[i][j] = pow(intenBuffer[i][j], r);
		}
	}

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			intenBuffer[i][j] = intenBuffer[i][j] * 255;
		}
	}

	OnGreylevelGreyscale();
}


void CMy201713074DIPView::OnGreylevelHistogramequalization()
{
	// TODO: Add your command handler code here
	//hsi 변환
	OnConversionRgbtohsi();

	//히스토그램 생성, 초기화
	unsigned int histogram[256];
	int sumHistogram[256];
	for (int i = 0; i < 256; i++) {
		histogram[i] = 0;
		sumHistogram[i] = 0;
	}

	//inten에 따라 histogram 값 입력
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			histogram[(int)intenBuffer[i][j]]++;
		}
	}

	//hitogram 출력용
	//퍼센트화
	int h[256];
	int iw = imgWidth;
	int ih = imgHeight;
	int pixcel = iw*ih;
	for (int i = 0; i < 256; i++) {
		h[i] = 1000 * histogram[i] / pixcel; 
		//전체 픽셀의 몇퍼센트인지, 원래 100 곱해야하는데 출력한 히스토그램이 너무 작아서 임의로 높이 키워줌..(*10)
	}
	//히스토그램 저장
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 100; j++) {
			if ((100 - h[i]) <= j) {
				histogramPrint[i][j] = 50;
			}
			else
			{
				histogramPrint[i][j] = 230;
			}
		}
	}
	viewtype = 2;
	//hitogram 출력용


	//누적합 계산
	int sum = 0;
	float scaleFactor = 255.0 / (imgHeight*imgWidth);
	
	for (int i = 0; i < 256; i++) {
		sum += histogram[i];
		sumHistogram[i] = (sum*scaleFactor) + 0.5;
	}

	//변환
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			intenBuffer[i][j] = sumHistogram[(int)intenBuffer[i][j]];
		}
	}

	OnGreylevelGreyscale();

	//Invalidate(TRUE);
}

float GaussianFilter(int filter, int i, int j)
{
	float f = 0.0;
	int y = 0;

	if (filter == 1) {
		float H[3][3] = {
			{ 1, 2, 1 },
			{ 2, 4, 2 },
			{ 1, 2, 1 }
		};
		for (int x = i - 1; x <= i + 1; x++) {
			f = f + intenBuffer[x][j - 1] * H[y][0] + intenBuffer[x][j] * H[y][1] + intenBuffer[x][j + 1] * H[y][2];
			y++;
		}
		f = f / 16;
	}
	else if (filter == 2) {
		float H[5][5] = {
			{ 1,4,7,4,1 },
			{ 4,16,26,16,4 },
			{ 7,26,41,26,7 },
			{ 4,16,26,16,4 },
			{ 1,4,7,4,1 }
		};
		for (int x = i - 2; x <= i + 2; x++) {
			f = f + intenBuffer[x][j - 2] * H[y][0] + intenBuffer[x][j - 1] * H[y][1] + intenBuffer[x][j] * H[y][2] + intenBuffer[x][j + 1] * H[y][3] + intenBuffer[x][j + 2] * H[y][4];
			y++;
		}
		f = f / 273;
	}
	else if (filter == 3) {
		float H[7][7] = {
			{1,6,15,20,15,6,1},
			{6,36,90,120,90,36,6},
			{15,90,225,300,225,90,15},
			{20,120,300,400,300,120,20},
			{ 15,90,225,300,225,90,15 },
			{ 6,36,90,120,90,36,6 },
			{ 1,6,15,20,15,6,1 }
		};
		for (int x = i - 3; x <= i + 3; x++) {
			f = f + intenBuffer[x][j - 3] * H[y][0] + intenBuffer[x][j - 2] * H[y][1] + intenBuffer[x][j - 1] * H[y][2]
				+ intenBuffer[x][j] * H[y][3] + intenBuffer[x][j + 1] * H[y][4] + intenBuffer[x][j + 2] * H[y][5] + intenBuffer[x][j + 3] * H[y][6];
			y++;
		}
		f = f / 4096;
	}
	
	return f;
}


void Gaussian(int filterSize) {//필터 크기 변수

	int filter = (filterSize - 1) / 2;

	float** filteredImage; //필터링한 이미지 저장할 버퍼[][]
	filteredImage = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		filteredImage[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	* 필터 크기: (i-filter,j-filter)~(i+filter, j+filter)
	* i: 0 ~ imgHeight
	* j: 0 ~ imgWidth
	*/
	float f = 0.0;

	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			filteredImage[i][j] = GaussianFilter(filter, i, j);
		}
	}

	//필터 적용
	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			intenBuffer[i][j] = filteredImage[i][j];
		}
	}
}


void CMy201713074DIPView::OnFilteringGaussian()
{
	// TODO: Add your command handler code here
	//hsi 변경
	OnConversionRgbtohsi();

	Gaussian(3);

	OnGreylevelGreyscale();
}


float MedianFilter(int filter, int i, int j)
{
	float f = 0.0;
	int y = 0;
	using namespace std;

	if (filter == 1) {
		float H[9];
		for (int x = i - 1; x <= i + 1; x++) {
			H[0 + y] = intenBuffer[x][j - 1];
			H[1 + y] = intenBuffer[x][j];
			H[2 + y] = intenBuffer[x][j + 1];
			y = y + 3;
		}
		sort(H + 0, H + 9);
		f = H[4];
	}
	else if (filter == 2) {
		float H[25];
		for (int x = i - 2; x <= i + 2; x++) {
			H[0 + y] = intenBuffer[x][j - 2];
			H[1 + y] = intenBuffer[x][j - 1];
			H[2 + y] = intenBuffer[x][j];
			H[3 + y] = intenBuffer[x][j + 1];
			H[4 + y] = intenBuffer[x][j + 2];
			y = y + 5;
		}
		sort(H + 0, H + 25);
		f = H[12];
	}
	else if (filter == 3) {
		float H[49];
		for (int x = i - 3; x <= i + 3; x++) {
			H[0 + y] = intenBuffer[x][j - 3];
			H[1 + y] = intenBuffer[x][j - 2];
			H[2 + y] = intenBuffer[x][j - 1];
			H[3 + y] = intenBuffer[x][j];
			H[4 + y] = intenBuffer[x][j + 1];
			H[5 + y] = intenBuffer[x][j + 2];
			H[6 + y] = intenBuffer[x][j + 3];
			y = y + 7;
		}
		sort(H + 0, H + 49);
		f = H[24];
	}

	return f;
}


void CMy201713074DIPView::OnFilteringMedian()
{
	// TODO: Add your command handler code here
	//hsi 변경
	OnConversionRgbtohsi();

	int filterSize = 7;	//필터 크기 변수
	int filter = (filterSize - 1) / 2;

	float** filteredImage; //필터링한 이미지 저장할 버퍼[][]
	filteredImage = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		filteredImage[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	* 필터 크기: (i-filter,j-filter)~(i+filter, j+filter)
	* i: 0 ~ imgHeight
	* j: 0 ~ imgWidth
	*/
	float f = 0.0;

	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			filteredImage[i][j] = MedianFilter(filter, i, j);
		}
	}

	//필터 적용
	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			intenBuffer[i][j] = filteredImage[i][j];
		}
	}

	OnGreylevelGreyscale();
}

float AverageFilter(int filter, int i, int j)
{
	float f = 0.0;
	switch (filter)
	{
	case 1:
		f = intenBuffer[i - 1][j - 1] + intenBuffer[i - 1][j] + intenBuffer[i - 1][j + 1]
			+ intenBuffer[i][j - 1] + intenBuffer[i][j] + intenBuffer[i][j + 1]
			+ intenBuffer[i + 1][j - 1] + intenBuffer[i + 1][j] + intenBuffer[i + 1][j + 1];
		f = f / 9;
		break;
	case 2:
		
		for (int x = i - 2; x <= i + 2; x++) {
			f = f + intenBuffer[x][j - 2] + intenBuffer[x][j - 1] + intenBuffer[x][j] + intenBuffer[x][j + 1] + intenBuffer[x][j + 2];
		}
		f = f / 25;
		break;
	case 3:
		for (int x = i - 3; x <= i + 3; x++) {
			f = f+ intenBuffer[x][j - 3] + intenBuffer[x][j - 2] + intenBuffer[x][j - 1] + intenBuffer[x][j] + intenBuffer[x][j + 1] + intenBuffer[x][j + 2] + intenBuffer[x][j + 3];
		}
		f = f / 49;
		break;
	default:
		break;
	}
	return f;
}

void CMy201713074DIPView::OnFilteringAverage()
{
	// TODO: Add your command handler code here
	//hsi 변경
	OnConversionRgbtohsi();

	int filterSize = 7;	//필터 크기 변수
	int filter = (filterSize - 1) / 2;

	float** filteredImage; //필터링한 이미지 저장할 버퍼[][]
	filteredImage = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		filteredImage[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	* 필터 크기: (i-filter,j-filter)~(i+filter, j+filter)
	* i: 0 ~ imgHeight
	* j: 0 ~ imgWidth
	*/
	float f = 0.0;

	for (int i = 0+filter; i < imgHeight-filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			filteredImage[i][j] = AverageFilter(filter, i, j);
		}
	}

	//필터 적용
	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			intenBuffer[i][j] = filteredImage[i][j];
		}
	}

	OnGreylevelGreyscale();
}


RGBQUAD ColorMedianFilter(int filter, int i, int j, int rgb)
{
	RGBQUAD f;
	int y = 0;
	using namespace std;

	if (filter == 1) {
		float H[9];
		for (int x = i - 1; x <= i + 1; x++) {
			H[0 + y] = rgbBuffer[x][j - 1].rgbRed;
			H[1 + y] = rgbBuffer[x][j].rgbRed;
			H[2 + y] = rgbBuffer[x][j + 1].rgbRed;
			y = y + 3;
		}
		sort(H + 0, H + 9);
		f.rgbRed = H[4];

		y = 0;
		for (int x = i - 1; x <= i + 1; x++) {
			H[0 + y] = rgbBuffer[x][j - 1].rgbGreen;
			H[1 + y] = rgbBuffer[x][j].rgbGreen;
			H[2 + y] = rgbBuffer[x][j + 1].rgbGreen;
			y = y + 3;
		}
		sort(H + 0, H + 9);
		f.rgbGreen = H[4];

		y = 0;
		for (int x = i - 1; x <= i + 1; x++) {
			H[0 + y] = rgbBuffer[x][j - 1].rgbBlue;
			H[1 + y] = rgbBuffer[x][j].rgbBlue;
			H[2 + y] = rgbBuffer[x][j + 1].rgbBlue;
			y = y + 3;
		}
		sort(H + 0, H + 9);
		f.rgbBlue = H[4];
	}
	else if (filter == 2) {//**************************************************
		float H[25];
		for (int x = i - 2; x <= i + 2; x++) {
			H[0 + y] = rgbBuffer[x][j - 2].rgbRed;
			H[1 + y] = rgbBuffer[x][j - 1].rgbRed;
			H[2 + y] = rgbBuffer[x][j].rgbRed;
			H[3 + y] = rgbBuffer[x][j + 1].rgbRed;
			H[4 + y] = rgbBuffer[x][j + 2].rgbRed;
			y = y + 5;
		}
		sort(H + 0, H + 25);
		f.rgbRed = H[12];

		y = 0;
		for (int x = i - 2; x <= i + 2; x++) {
			H[0 + y] = rgbBuffer[x][j - 2].rgbGreen;
			H[1 + y] = rgbBuffer[x][j - 1].rgbGreen;
			H[2 + y] = rgbBuffer[x][j].rgbGreen;
			H[3 + y] = rgbBuffer[x][j + 1].rgbGreen;
			H[4 + y] = rgbBuffer[x][j + 2].rgbGreen;
			y = y + 5;
		}
		sort(H + 0, H + 25);
		f.rgbGreen = H[12];

		y = 0;
		for (int x = i - 2; x <= i + 2; x++) {
			H[0 + y] = rgbBuffer[x][j - 2].rgbBlue;
			H[1 + y] = rgbBuffer[x][j - 1].rgbBlue;
			H[2 + y] = rgbBuffer[x][j].rgbBlue;
			H[3 + y] = rgbBuffer[x][j + 1].rgbBlue;
			H[4 + y] = rgbBuffer[x][j + 2].rgbBlue;
			y = y + 5;
		}
		sort(H + 0, H + 25);
		f.rgbBlue = H[12];
	}
	else if (filter == 3) {//**************************************************
		float H[49];
		for (int x = i - 3; x <= i + 3; x++) {
			H[0 + y] = rgbBuffer[x][j - 3].rgbRed;
			H[1 + y] = rgbBuffer[x][j - 2].rgbRed;
			H[2 + y] = rgbBuffer[x][j - 1].rgbRed;
			H[3 + y] = rgbBuffer[x][j].rgbRed;
			H[4 + y] = rgbBuffer[x][j + 1].rgbRed;
			H[5 + y] = rgbBuffer[x][j + 2].rgbRed;
			H[6 + y] = rgbBuffer[x][j + 3].rgbRed;
			y = y + 7;
		}
		sort(H + 0, H + 49);
		f.rgbRed = H[24];

		y = 0;
		for (int x = i - 3; x <= i + 3; x++) {
			H[0 + y] = rgbBuffer[x][j - 3].rgbGreen;
			H[1 + y] = rgbBuffer[x][j - 2].rgbGreen;
			H[2 + y] = rgbBuffer[x][j - 1].rgbGreen;
			H[3 + y] = rgbBuffer[x][j].rgbGreen;
			H[4 + y] = rgbBuffer[x][j + 1].rgbGreen;
			H[5 + y] = rgbBuffer[x][j + 2].rgbGreen;
			H[6 + y] = rgbBuffer[x][j + 3].rgbGreen;
			y = y + 7;
		}
		sort(H + 0, H + 49);
		f.rgbGreen = H[24];

		y = 0;
		for (int x = i - 3; x <= i + 3; x++) {
			H[0 + y] = rgbBuffer[x][j - 3].rgbBlue;
			H[1 + y] = rgbBuffer[x][j - 2].rgbBlue;
			H[2 + y] = rgbBuffer[x][j - 1].rgbBlue;
			H[3 + y] = rgbBuffer[x][j].rgbBlue;
			H[4 + y] = rgbBuffer[x][j + 1].rgbBlue;
			H[5 + y] = rgbBuffer[x][j + 2].rgbBlue;
			H[6 + y] = rgbBuffer[x][j + 3].rgbBlue;
			y = y + 7;
		}
		sort(H + 0, H + 49);
		f.rgbBlue = H[24];
		
	}

	return f;
}

void CMy201713074DIPView::OnFilteringColormedian()
{
	// TODO: Add your command handler code here
	if (rgbBuffer == NULL)
		OnImgloadJpg();

	int filterSize = 5;	//필터 크기 변수
	int filter = (filterSize - 1) / 2;

	RGBQUAD** filteredImage; //필터링한 이미지 저장할 버퍼[][]
	filteredImage = new RGBQUAD*[imgHeight];
	for (int i = 0; i < imgHeight; i++)
		filteredImage[i] = new RGBQUAD[imgWidth];

	/*
	* 필터 계산
	* 필터 크기: (i-filter,j-filter)~(i+filter, j+filter)
	* i: 0 ~ imgHeight
	* j: 0 ~ imgWidth
	*/

	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			filteredImage[i][j] = ColorMedianFilter(filter, i, j, 0);
		}
	}

	//필터 적용
	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			rgbBuffer[i][j] = filteredImage[i][j];
		//	rgbBuffer[i][j].rgbGreen = filteredImage[i][j].rgbGreen;
		//	rgbBuffer[i][j].rgbBlue = filteredImage[i][j].rgbBlue;
		}
	}

	Invalidate(TRUE);
}


float HighboostFilter(int mask, int i, int j)
{
	float f = 0.0;
	int y = 0;

	if (mask == 1) {
		float H[3][3] = {
			{0,-1,0},
			{-1,4,-1},
			{0,-1,0}
		};
		for (int x = i - 1; x <= i + 1; x++) {
			f = f + intenBuffer[x][j - 1] * H[y][0] + intenBuffer[x][j] * H[y][1] + intenBuffer[x][j + 1] * H[y][2];
			y++;
		}
	}
	else if (mask == 2) {
		float H[3][3] = {
			{-1,-1,-1},
			{-1,8,-1},
			{-1,-1,-1}
		};
		for (int x = i - 1; x <= i + 1; x++) {
			f = f + intenBuffer[x][j - 1] * H[y][0] + intenBuffer[x][j] * H[y][1] + intenBuffer[x][j + 1] * H[y][2];
			y++;
		}
	}

	return f;
}

void CMy201713074DIPView::OnFilteringHighboost() //????????????????????????????
{
	// TODO: Add your command handler code here
	//hsi 변경
	OnConversionRgbtohsi();

	int mask = 1;	//마스크 종류
	float a = 1.2;	//A값

	float** filteredImage; //필터링한 이미지 저장할 버퍼[][]
	filteredImage = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		filteredImage[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	* 필터 크기: (i-filter,j-filter)~(i+filter, j+filter)
	* i: 0 ~ imgHeight
	* j: 0 ~ imgWidth
	*/
	float f = 0.0;

	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			f = HighboostFilter(mask, i, j);
			if (f > 128) {
				filteredImage[i][j] = intenBuffer[i][j] * a + HighboostFilter(mask, i, j);
			}
			else {
				filteredImage[i][j] = intenBuffer[i][j] * a - HighboostFilter(mask, i, j);
			}
		}
	}

	//필터 적용
	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			intenBuffer[i][j] = filteredImage[i][j];
		}
	}

	OnGreylevelGreyscale();
}


void CMy201713074DIPView::OnEdgePrewitt()
{
	// TODO: Add your command handler code here
	//hsi 변경
	OnConversionRgbtohsi();

	float** edgeImageX; //x축 필터링한 이미지 저장할 버퍼[][]
	edgeImageX = new float*[imgHeight];
	float** edgeImageY; //y축 필터링한 이미지 저장할 버퍼[][]
	edgeImageY = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		edgeImageX[i] = new float[imgWidth];
		edgeImageY[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	*/
	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			//edgeImageX[i][j] = intenBuffer[i][j + 1] - intenBuffer[i][j-1];
			//edgeImageY[i][j] = intenBuffer[i-1][j] - intenBuffer[i + 1][j];
			edgeImageX[i][j] = (intenBuffer[i - 1][j + 1] - intenBuffer[i - 1][j - 1]
				+ intenBuffer[i][j + 1] - intenBuffer[i][j - 1]
				+ intenBuffer[i + 1][j + 1] - intenBuffer[i + 1][j - 1]) / 3;
			edgeImageY[i][j] = (intenBuffer[i - 1][j - 1] - intenBuffer[i + 1][j - 1]
				+ intenBuffer[i - 1][j] - intenBuffer[i + 1][j]
				+ intenBuffer[i - 1][j + 1] - intenBuffer[i + 1][j + 1]) / 3;
		}
	}

	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			intenBuffer[i][j] = sqrt(pow(edgeImageX[i][j], 2) + pow(edgeImageY[i][j], 2));
		}
	}

	OnGreylevelGreyscale();
}


void CMy201713074DIPView::OnEdgeSobel()
{
	// TODO: Add your command handler code here
	OnConversionRgbtohsi();

	float** edgeImageX; //x축 필터링한 이미지 저장할 버퍼[][]
	edgeImageX = new float*[imgHeight];
	float** edgeImageY; //y축 필터링한 이미지 저장할 버퍼[][]
	edgeImageY = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		edgeImageX[i] = new float[imgWidth];
		edgeImageY[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	*/
	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			edgeImageX[i][j] = (intenBuffer[i-1][j + 1] - intenBuffer[i-1][j - 1]
				+ 2*intenBuffer[i][j + 1] - 2*intenBuffer[i][j - 1]
				+ intenBuffer[i+1][j + 1] - intenBuffer[i+1][j - 1]) / 3;
			edgeImageY[i][j] = (intenBuffer[i - 1][j-1] - intenBuffer[i + 1][j-1]
				+ 2*intenBuffer[i - 1][j] - 2*intenBuffer[i + 1][j]
				+ intenBuffer[i - 1][j+1] - intenBuffer[i + 1][j+1]) / 3;
		}
	}

	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			intenBuffer[i][j] = sqrt(pow(edgeImageX[i][j], 2) + pow(edgeImageY[i][j], 2));
		}
	}

	OnGreylevelGreyscale();
}


float LaplacianFilter(int filter, int i, int j) {
	float f = 0.0;
	int y = 0;

	if (filter == 1) {
		float H[3][3] = {
			{0,1,0},
			{1,-4,1},
			{0,1,0}
		};
		//f = intenBuffer[i + 1][j] + intenBuffer[i - 1][j] + intenBuffer[i][j + 1] + intenBuffer[i][j - 1] - 4 * intenBuffer[i][j];
		
		for (int x = i - 1; x <= i + 1; x++) {
			f = f + intenBuffer[x][j - 1] * H[y][0] + intenBuffer[x][j] * H[y][1] + intenBuffer[x][j + 1] * H[y][2];
			y++;
		}
	}
	else if (filter == 2) {
		float H[5][5] = {
			{0,0,-1,0,0},
			{0,-1,-2,-1,0},
			{-1,-2,16,-2,-1},
			{ 0,-1,-2,-1,0 },
			{ 0,0,-1,0,0 }
		};
		for (int x = i - 2; x <= i + 2; x++) {
			f = f - intenBuffer[x][j - 2] * H[y][0] - intenBuffer[x][j - 1] * H[y][1] - intenBuffer[x][j] * H[y][2] - intenBuffer[x][j + 1] * H[y][3] - intenBuffer[x][j + 2] * H[y][4];
			y++;
		}
	}
	else if (filter == 3) {
		float H[7][7] = {
			{ 1,6,15,20,15,6,1 },
			{ 6,36,90,120,90,36,6 },
			{ 15,90,225,300,225,90,15 },
			{ 20,120,300,400,300,120,20 },
			{ 15,90,225,300,225,90,15 },
			{ 6,36,90,120,90,36,6 },
			{ 1,6,15,20,15,6,1 }
		};
		for (int x = i - 3; x <= i + 3; x++) {
			f = f + intenBuffer[x][j - 3] * H[y][0] + intenBuffer[x][j - 2] * H[y][1] + intenBuffer[x][j - 1] * H[y][2]
				+ intenBuffer[x][j] * H[y][3] + intenBuffer[x][j + 1] * H[y][4] + intenBuffer[x][j + 2] * H[y][5] + intenBuffer[x][j + 3] * H[y][6];
			y++;
		}
	}

	return f;
}

void CMy201713074DIPView::OnEdgeLog()
{
	// TODO: Add your command handler code here
	//hsi 변경
	OnConversionRgbtohsi();

	int filterSize = 3;	//필터 크기 변수
	int filter = (filterSize - 1) / 2;

	Gaussian(filterSize+2);

	float** edgeImage; //x축 필터링한 이미지 저장할 버퍼[][]
	edgeImage = new float*[imgHeight];
	float** edgeImageY; //y축 필터링한 이미지 저장할 버퍼[][]
	edgeImageY = new float*[imgHeight];
	for (int i = 0; i < imgHeight; i++) {
		edgeImage[i] = new float[imgWidth];
		edgeImageY[i] = new float[imgWidth];
	}

	/*
	* 필터 계산
	*/
	for (int i = 0 + filter; i < imgHeight - filter; i++) {
		for (int j = 0 + filter; j < imgWidth - filter; j++) {
			//edgeImageX[i][j] = intenBuffer[i][j + 1] - intenBuffer[i][j-1];
			//edgeImageY[i][j] = intenBuffer[i-1][j] - intenBuffer[i + 1][j];
			edgeImage[i][j] = LaplacianFilter(filter, i, j);
		}
	}

	for (int i = 0 + 1; i < imgHeight - 1; i++) {
		for (int j = 0 + 1; j < imgWidth - 1; j++) {
			intenBuffer[i][j] = edgeImage[i][j];
		}
	}

	/*
	for (int i = 0 + 1; i < imgHeight - 1; i++) {
	for (int j = 0 + 1; j < imgWidth - 1; j++) {
	//edgeImageX[i][j] = intenBuffer[i][j + 1] - intenBuffer[i][j-1];
	//edgeImageY[i][j] = intenBuffer[i-1][j] - intenBuffer[i + 1][j];
	edgeImageX[i][j] = intenBuffer[i][j - 1] - 2 * intenBuffer[i][j] + intenBuffer[i][j + 1];
	edgeImageY[i][j] = intenBuffer[i - 1][j] - 2 * intenBuffer[i][j] + intenBuffer[i + 1][j];
	}
	}

	for (int i = 0 + 1; i < imgHeight - 1; i++) {
	for (int j = 0 + 1; j < imgWidth - 1; j++) {
	intenBuffer[i][j] = sqrt(pow(edgeImageX[i][j], 2) + pow(edgeImageY[i][j], 2));
	}
	}
	*/

	OnGreylevelGreyscale();
}


void CMy201713074DIPView::OnVideoAvi()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File(*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;

	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	String filename(strAtl);

	//파일 로드
	cv::VideoCapture Capture;
	Capture.open(filename);
	//파일에 문제가 있을 경우 에러 메시지 표시 후 종료
	if (!Capture.isOpened())
		AfxMessageBox("Error Video");

	for (;;) {
		Mat frame;
		Capture >> frame;
		if (frame.data == nullptr)
			break;
		imshow("video", frame);
		if (waitKey(30) >= 0)
			break;
	}

	AfxMessageBox("Completed");
}


void CMy201713074DIPView::OnVideoMp4()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, ".mp4", NULL, NULL, "MP4 File(*.mp4)|*.mp4||");
	if (IDOK != dlg.DoModal())
		return;

	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	String filename(strAtl);

	//파일 로드
	cv::VideoCapture Capture;
	Capture.open(filename);
	//파일에 문제가 있을 경우 에러 메시지 표시 후 종료
	if (!Capture.isOpened())
		AfxMessageBox("Error Video");

	for (;;) {
		Mat frame;
		Capture >> frame;
		if (frame.data == nullptr)
			break;
		imshow("video", frame);
		if (waitKey(30) >= 0)
			break;
	}

	AfxMessageBox("Completed");
}


void CMy201713074DIPView::OnVideoLucaskanade()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, ".mp4", NULL, NULL, "MP4 File(*.mp4)|*.mp4||");
	if (IDOK != dlg.DoModal())
		return;

	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	String filename(strAtl);

	//파일 로드
	cv::VideoCapture Capture;
	Capture.open(filename);
	//파일에 문제가 있을 경우 에러 메시지 표시 후 종료
	if (!Capture.isOpened())
		AfxMessageBox("Error Video");

	Mat frame_prev;
	Mat frame;
	Mat frame_color;
	/*

	for (;;) {
		Capture >> frame;
		if (frame.data == nullptr)
			break;
		calcOpticalFlowFarneback(frame_prev, frame, frame_color, 0.5, 3, 21, 20, 5, 1.1, 0);
		imshow("video", frame_color);
		if (waitKey(30) >= 0)
			break;
		frame_prev = frame.clone();
	}
	*/

	AfxMessageBox("Completed");
}
