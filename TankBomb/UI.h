#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>
#include <assert.h>
#include "DXTrace.h"

#include "Keyboard.h"

/*
TODO:
	Animation
*/


template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
//
// 实验部分
//

class UiObject
{

};

class Container
{
public:
	virtual void addChild();

};

class Layout
{
	
};















//
//
//
struct Color
{
	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	D2D1_COLOR_F getColor()
	{
		return D2D1::ColorF(r, g, b, a);
	}

	ComPtr<ID2D1SolidColorBrush> getColorBrush(ID2D1RenderTarget* renderTarget)
	{
		ComPtr<ID2D1SolidColorBrush> colorBrush;
		HR(renderTarget->CreateSolidColorBrush(getColor(), colorBrush.GetAddressOf()));
		return colorBrush;
	}

	float r;
	float g;
	float b;
	float a;
};

struct Rect
{
	Rect(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	D2D1_RECT_F getRect()
	{
		return D2D1::RectF(x, y, x + width, y + height);
	}

	float x;
	float y;
	float width;
	float height;
};

class Font
{
public:
	Font() : size(18), fontFamily(L"宋体")
	{
		//textFormat
	}
	Font(float sz, std::wstring family): size(sz), fontFamily(family)
	{
		/*writeFactory = factory;
		size = sz;
		fontFamily = family;
		writeFactory->CreateTextFormat(fontFamily.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"zh-cn",
			textFormat.GetAddressOf());*/
	}
	float size;
	std::wstring fontFamily;
};

class TextFactory;
class Text
{
public:
	friend class TextFactory;
	Text(std::wstring str, Rect rect, Font& font, Color& color) : textFont(font), textColor(color), content(str), textRect(rect)
	{
		/*this->renderTarget = renderTarget;
		this->color = color;
		this->content = str;*/
	}

	void draw()
	{
		ComPtr<ID2D1SolidColorBrush> colorBrush;
		HR(renderTarget->CreateSolidColorBrush(textColor.getColor(),
			colorBrush.GetAddressOf()));
		//renderTarget->DrawTextW(content.c_str(), (UINT32)content.size(), textFormat.Get(),
		//	textRect.getRect(), colorBrush.Get());
		DWRITE_TEXT_METRICS metrics;
		textLayout->GetMetrics(&metrics);
		renderTarget->DrawTextLayout(D2D1::Point2F(textRect.x, textRect.y), textLayout.Get(), textColor.getColorBrush(renderTarget).Get());
	}

	
private:
	ID2D1RenderTarget* renderTarget;
	ComPtr<IDWriteTextLayout> textLayout;
	ComPtr<IDWriteTextFormat> textFormat;
	Rect textRect;
	std::wstring content;
	Color textColor;
	Font textFont;
};


class TextFactory
{
public:
	TextFactory()
	{

	}

	void init(ID2D1RenderTarget** pRenderTarget, IDWriteFactory** writeFactory)
	{
		this->pRenderTarget = pRenderTarget;
		this->pWriteFactory = writeFactory;
	}

	Text createText(std::wstring str, Rect rect, Font& font, Color& color)
	{
		Text text(str, rect, font, color);
		text.renderTarget = *pRenderTarget;
		(*pWriteFactory)->CreateTextFormat(font.fontFamily.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, font.size, L"zh-cn",
				text.textFormat.GetAddressOf());
		//超过指定显示矩形部分的裁剪设置
		DWRITE_TRIMMING trim;
		trim.granularity = DWRITE_TRIMMING_GRANULARITY_WORD;
		trim.delimiter = 1;
		trim.delimiterCount = 10;
		ComPtr<IDWriteInlineObject> trim2;
		(*pWriteFactory)->CreateEllipsisTrimmingSign(text.textFormat.Get(), trim2.GetAddressOf());
		text.textFormat->SetTrimming(&trim, trim2.Get());
		text.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		//文本布局
		(*pWriteFactory)->CreateTextLayout(str.c_str(), str.size(), text.textFormat.Get(), rect.width, 
			rect.height, text.textLayout.GetAddressOf());
		return text;
	}

private:
	ID2D1RenderTarget** pRenderTarget;
	IDWriteFactory** pWriteFactory;
};


class UiManager
{
public:
	UiManager() = default;
	~UiManager() = default;

	void init()
	{
		HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pd2dFactory.GetAddressOf()));
		HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(m_pdwriteFactory.GetAddressOf())));

		textFactory.init(m_pd2dRenderTarget.GetAddressOf(), m_pdwriteFactory.GetAddressOf());
	}
	void releaseResources()
	{
		assert(m_pd2dFactory);
		assert(m_pdwriteFactory);
		//m_pColorBrush.Reset();
		m_pd2dRenderTarget.Reset();
	}
	void obtainResources(IDXGISwapChain* swapChain, int width, int height)
	{
		m_Width = width;
		m_Height = height;
		ComPtr<IDXGISurface> surface;
		HR(swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, m_pd2dRenderTarget.GetAddressOf());
		surface.Reset();

		if (hr == E_NOINTERFACE)
		{
			OutputDebugStringW(L"\n警告：Direct2D与Direct3D互操作性功能受限，你将无法看到文本信息。现提供下述可选方法：\n"
				L"1. 对于Win7系统，需要更新至Win7 SP1，并安装KB2670838补丁以支持Direct2D显示。\n"
				L"2. 自行完成Direct3D 10.1与Direct2D的交互。详情参阅："
				L"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
				L"3. 使用别的字体库，比如FreeType。\n\n");
		}
		else if (hr == S_OK)
		{
			// 创建固定颜色刷和文本格式
			/*HR(m_pd2dRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(0.7f, 0.7f, 1.0f),
				m_pColorBrush.GetAddressOf()));
			HR(m_pdwriteFactory->CreateTextFormat(L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"zh-cn",
				m_pTextFormat.GetAddressOf()));*/
		}
		else
		{
			// 报告异常问题
			assert(m_pd2dRenderTarget);
		}
	}
	void drawFps()
	{

	}
	void drawHud()
	{
		m_pd2dRenderTarget->BeginDraw();

		//背景
		/*Color color(0.0f, 0.0f, 0.0f, 0.8f);
		Rect rect(0.0f, 0.0f, (float)m_Width, (float)m_Height);
		m_pd2dRenderTarget->FillRectangle(rect.getRect(),
			color.getColorBrush(m_pd2dRenderTarget.Get()).Get());*/

		/*L"当前摄像机模式: 第三人称（按1切换第一人称）  Esc退出\n"
			L"鼠标移动控制视野 滚轮控制第三人称观察距离\n"
			L"W，S前进，A,D转向 \n"
			L"left,right旋转炮台，up,down调整角度 \n";*/

		//提示文本
		Rect rect4(0, 0, 350, 300);
		Font font4(15.0f, L"宋体");
		Color color4(1.0f, 1.0f, 1.0f);
		std::wstring str = 
			L"当前摄像机模式: 第三人称（按1切换第一人称）  Esc退出\n"
			L"鼠标移动控制视野 滚轮控制第三人称观察距离\n"
			L"W，S前进，A,D转向 \n"
			L"left,right旋转炮台，up,down调整角度 \n";
		Text text4 = textFactory.createText(str.c_str(), rect4, font4, color4);
		text4.draw();

		//准星

		m_pd2dRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F((float)m_Width / 2, (float)m_Height / 2), 20, 20),
			Color(0.3f, 1.0f, 0.3f, 0.6f).getColorBrush(m_pd2dRenderTarget.Get()).Get(), 3);
		m_pd2dRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F((float)m_Width / 2, (float)m_Height / 2), 2, 2),
			Color(0.3f, 1.0f, 0.3f, 0.6f).getColorBrush(m_pd2dRenderTarget.Get()).Get());

		//HP
		float x, y, width, height;
		Color color1(0.6f, 0.0f, 0.0f, 1.0f);
		width = 250.0f;
		height = 30.0f;
		x = m_Width - width;
		y = m_Height - height - 20.0f;
		Rect rect1(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect1.getRect(),
			color1.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font1(20.0f, L"黑体");
		Color color11(1.0f, 1.0f, 1.0f);
		Text text1 = textFactory.createText(L"HP: 100%", rect1, font1, color11);
		text1.draw();

		m_pd2dRenderTarget->EndDraw();
	}
	void drawHud2()
	{
		m_pd2dRenderTarget->BeginDraw();

		//背景
		Color color(0.0f, 0.0f, 0.0f, 0.8f);
		Rect rect(0.0f, 0.0f, (float)m_Width, (float)m_Height);
		m_pd2dRenderTarget->FillRectangle(rect.getRect(),
			color.getColorBrush(m_pd2dRenderTarget.Get()).Get());


		//准星

		m_pd2dRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(m_Width / 2, m_Height / 2), 100, 100),
			Color(0.3f, 1.0f, 0.3f, 0.6f).getColorBrush(m_pd2dRenderTarget.Get()).Get(), 3);
		m_pd2dRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(m_Width / 2, m_Height / 2), 2, 2),
			Color(0.3f, 1.0f, 0.3f, 0.6f).getColorBrush(m_pd2dRenderTarget.Get()).Get());


		m_pd2dRenderTarget->EndDraw();
	}
	void drawMainMenu()
	{
		m_pd2dRenderTarget->BeginDraw();
		//背景
		Color color(0.0f, 0.0f, 0.0f, 0.8f);
		Rect rect(0.0f, 0.0f, (float)m_Width, (float)m_Height);
		m_pd2dRenderTarget->FillRectangle(rect.getRect(), 
			color.getColorBrush(m_pd2dRenderTarget.Get()).Get());

		//Logo
		float x, y, width, height;
		Color color1(0.5f, 0.5f, 0.5f, 1.0f);
		width = 400.0f;
		height = 150.0f;
		x = (m_Width - width) / 2;
		//y = (m_Height - height) / 5;
		y = (m_Height - 420) / 2;
		Rect rect1(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect1.getRect(),
			color1.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font1(85.0f, L"黑体");
		Color color11(1.0f, 1.0f, 1.0f);
		Text text1 = textFactory.createText(L"TankBomb", rect1, font1, color11);
		text1.draw();

		//StartButton
		Color color2(0.5f, 0.5f, 0.5f, 1.0f);
		width = 200.0f;
		height = 50.0f;
		x = (m_Width - width) / 2;
		y = rect1.y + rect1.height + 40.0f;
		Rect rect2(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect2.getRect(),
			color2.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font2(30.0f, L"黑体");
		Color color21(1.0f, 1.0f, 1.0f);
		Text text2 = textFactory.createText(L"🍋Start🍋", rect2, font2, color21);
		text2.draw();

		//SettingButton
		Color color3(0.5f, 0.5f, 0.5f, 1.0f);
		width = 200.0f;
		height = 50.0f;
		x = (m_Width - width) / 2;
		y = rect2.y + rect2.height + 40.0f;
		Rect rect3(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect3.getRect(),
			color3.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font3(30.0f, L"黑体");
		Color color31(1.0f, 1.0f, 1.0f);
		Text text3 = textFactory.createText(L"🍎Setting🍎", rect3, font3, color31);
		text3.draw();

		//QuitButton
		Color color4(0.5f, 0.5f, 0.5f, 1.0f);
		width = 200.0f;
		height = 50.0f;
		x = (m_Width - width) / 2;
		y = rect3.y + rect3.height + 40.0f;
		Rect rect4(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect4.getRect(),
			color4.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font4(30.0f, L"黑体");
		Color color41(1.0f, 1.0f, 1.0f);
		Text text4 = textFactory.createText(L"🍉Quit🍉", rect4, font4, color41);
		text4.draw();



		//m_pd2dRenderTarget->DrawTextLayout()
		/*Font font1(20.0f, L"宋体");
		Color color1(1.0f, 0.5f, 0.5f);
		Color color2(1.0f, 1.0f, 0.8f);
		Rect rect1(100.0f, 100.0f, 100.0f, 100.0f);
		Text text1 = textFactory.createText(L"开始游戏发四三一二一二一二一二一二一二一二一二一二一二一二一二一", rect1, font1, color1);
		m_pd2dRenderTarget->FillRectangle(rect1.getRect(), color2.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		text1.draw();*/

		m_pd2dRenderTarget->EndDraw();
	}
	void drawPauseMenu()
	{
		m_pd2dRenderTarget->BeginDraw();
		float x, y, width, height;
		//ContinueButton
		Color color1(0.5f, 0.5f, 0.5f, 1.0f);
		width = 200.0f;
		height = 50.0f;
		x = (m_Width - width) / 2;
		y = (m_Height - 230) / 2;
		Rect rect1(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect1.getRect(),
			color1.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font1(30.0f, L"黑体");
		Color color11(1.0f, 1.0f, 1.0f);
		Text text1 = textFactory.createText(L"Continue", rect1, font1, color11);
		text1.draw();

		//SettingButton
		Color color2(0.5f, 0.5f, 0.5f, 1.0f);
		width = 200.0f;
		height = 50.0f;
		x = (m_Width - width) / 2;
		y = rect1.y + rect1.height + 40.0f;
		Rect rect2(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect2.getRect(),
			color2.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font2(30.0f, L"黑体");
		Color color21(1.0f, 1.0f, 1.0f);
		Text text2 = textFactory.createText(L"Setting", rect2, font2, color21);
		text2.draw();

		//ReturnButton
		Color color3(0.5f, 0.5f, 0.5f, 1.0f);
		width = 200.0f;
		height = 50.0f;
		x = (m_Width - width) / 2;
		y = rect2.y + rect2.height + 40.0f;
		Rect rect3(x, y, width, height);
		m_pd2dRenderTarget->FillRectangle(rect3.getRect(),
			color3.getColorBrush(m_pd2dRenderTarget.Get()).Get());
		Font font3(30.0f, L"黑体");
		Color color31(1.0f, 1.0f, 1.0f);
		Text text3 = textFactory.createText(L"Quit", rect3, font3, color31);
		text3.draw();
		m_pd2dRenderTarget->EndDraw();
	}

	void draw()
	{
		if (m_pd2dRenderTarget != nullptr)
		{
			m_pd2dRenderTarget->BeginDraw();
			Font font(20.0f, L"黑体");
			Color color(0.8f, 0.8f, 1.0f);
			Rect rect(0.0f, 0.0f, 200.0f, 100.0f);
			Text text = textFactory.createText(L"你妈炸了好吧傻逼", rect, font, color);
			text.draw();

			Font font1(20.0f, L"黑体");
			Color color1(1.0f, 0.5f, 0.5f);
			Rect rect1(100.0f, 100.0f, 200.0f, 100.0f);
			Text text1 = textFactory.createText(L"你妈才炸了", rect1, font1, color1);
			//变换
			D2D1_MATRIX_3X2_F origin;
			static float angle = 0.0f;
			m_pd2dRenderTarget->GetTransform(&origin);
			//m_pd2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(100.0f, 100.0f)));
			m_pd2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(angle, 0.0f));
			angle += 0.01f;
			text1.draw();
			m_pd2dRenderTarget->SetTransform(origin);

			//m_pd2dRenderTarget->DrawRectangle(D2D1::RectF(100, 100, 400, 400),m_pColorBrush.Get(), 13.0f);
			//m_pd2dRenderTarget->FillRectangle(D2D1::RectF(200, 200, 300, 300), m_pColorBrush.Get());
			m_pd2dRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(400, 300), 20, 20), 
				Color(0.7f, 0.7f, 1.0f).getColorBrush(m_pd2dRenderTarget.Get()).Get(), 5);
			
			//ID2D1Geometry* g = &D2D1::Ellipse(D2D1::Point2F(400, 300), 20, 20);
			
			//m_pd2dRenderTarget->DrawGeometry()
		
			HR(m_pd2dRenderTarget->EndDraw());
		}
	}
	void render()
	{
		//draw();
		//drawMainMenu();
		//drawHud();
		//drawHud2();
		//drawPauseMenu();
		drawHud();
	}
	void update(float dt);
private:
	//TODO
	bool isThirdPerson = false;
	bool isX2 = false;
	//

	//Keyboard keyboard;
	//
	TextFactory textFactory;

	ComPtr<ID2D1Factory> m_pd2dFactory;							// D2D工厂
	ComPtr<ID2D1RenderTarget> m_pd2dRenderTarget;				// D2D渲染目标
	ComPtr<IDWriteFactory> m_pdwriteFactory;					// DWrite工厂

	int m_Width;												// 视口宽度
	int m_Height;												// 视口高度
};