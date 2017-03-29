#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"

#include "mt_cef.h"

#include "irrlicht.h" // createDevice
#include "irrlichttypes_extrabloated.h"

/* MinetestBrowser */
MinetestBrowser::MinetestBrowser() {
	;
}

MinetestBrowser::~MinetestBrowser() {
	;
}

MinetestBrowser* m_browser = NULL;
int skippy = 0;

// static
MinetestBrowser* MinetestBrowser::GetInstance()
{
	CHECK(m_browser);
	return m_browser;
}

// static
bool MinetestBrowser::IsInitialized()
{
	return (m_browser != NULL);
}

// static
void MinetestBrowser::Initialize()
{
	CHECK(!m_browser);
	skippy = 0;
	m_browser = new MinetestBrowser();

/*    char  cefarg0[] = "./cefclient";
    char  cefarg1[] = "enable-gpu";
    char* cefargv[] = { &cefarg0[0], &cefarg1[0], NULL };
    int   cefargc   = (int)(sizeof(cefargv) / sizeof(cefargv[0])) - 1;

	CefMainArgs args(cefargc, cefargv);
*/
	CefMainArgs args;
	CefSettings settings;

	settings.no_sandbox = true;
	settings.single_process = false;
	settings.multi_threaded_message_loop = false;
	settings.windowless_rendering_enabled = true;
	settings.command_line_args_disabled = true;
//	settings.windowless_frame_rate = 30;
//	settings.log_severity = LOGSEVERITY_VERBOSE;
	CefString(&settings.log_file).FromASCII("cef.log");
	CefString(&settings.browser_subprocess_path).FromASCII("./cefclient");

	// Aaaaaand ... GO!
	CefInitialize(args, settings, nullptr, nullptr);

}

// static
void MinetestBrowser::Shutdown()
{
	CefShutdown();
}

// static
void MinetestBrowser::Update()
{
	CHECK(m_browser);
    if (skippy >= 1)
    {
        CefDoMessageLoopWork();
        skippy = 0;
    }
    skippy++;
}

/* ITexture methods */
WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	video::ITexture *texture, v2s32 geom)
{
	CHECK(m_browser);

	WebPage* webPage = new WebPage(name, driver, texture, geom.X, geom.Y);
	m_webPages[name] = webPage;

	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	video::ITexture *texture, v2s32 geom, std::string url)
{
	CHECK(m_browser);

	WebPage* webPage = CreateWebPage(name, driver, texture, geom);
	webPage->Open(url);

	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	video::ITexture *texture, core::dimension2d<u32> dimension)
{
	CHECK(m_browser);

	WebPage* webPage = new WebPage(name, driver, texture, dimension.Width, dimension.Height);
	m_webPages[name] = webPage;

	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	video::ITexture *texture, core::dimension2d<u32> dimension, std::string url)
{
	CHECK(m_browser);

	WebPage* webPage = CreateWebPage(name, driver, texture, dimension);
	webPage->Open(url);

	return webPage;
}

/* IImage methods */
WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	video::IImage *image, core::dimension2d<u32> dimension)
{
	CHECK(m_browser);

	WebPage* webPage = new WebPage(name, driver, image,
        dimension.Width, dimension.Height);
	m_webPages[name] = webPage;

	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	video::IImage *image, core::dimension2d<u32> dimension, std::string url)
{
	CHECK(m_browser);

	WebPage* webPage = CreateWebPage(name, driver, image, dimension);
	webPage->Open(url);

	return webPage;
}

/* IGUIEnvironment methods */
WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom)
{
	CHECK(m_browser);

	video::ITexture *texture =
		driver->addTexture(core::dimension2d<u32>(geom.X, geom.Y),
		   io::path("browsertexture"),
		   irr::video::ECF_A8R8G8B8);

	irr::gui::IGUIImage* image = guiEnv->addImage(texture, core::position2d<s32>(pos.X, pos.Y));

	WebPage* webPage = new WebPage(name, driver, image, texture, geom.X, geom.Y);
	webPage->SetSize(geom.X, geom.Y);
	m_webPages[name] = webPage;

	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
	irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom, std::string url)
{
	CHECK(m_browser);

	WebPage* webPage = CreateWebPage(name, driver, guiEnv, pos, geom);
	webPage->Open(url);

	return webPage;
}

void MinetestBrowser::CloseWebPage(std::string name)
{
	CHECK(m_browser);

	WebPage* webPage = m_webPages[name];
	if (webPage == NULL) return;

	webPage->Close();
	m_webPages.erase(name);
	delete webPage;
}

/* WebPage */
WebPage::WebPage(std::string name, video::IVideoDriver *driver,
		irr::gui::IGUIImage *guiImage, video::ITexture *texture,
		int width, int height)
	: m_name(name)
	, m_width(width)
	, m_height(height)
	, m_driver(driver)
	, m_guiImage(guiImage)
	, m_texture(texture)
{
	CHECK(guiImage);
	CHECK(texture);
	MinetestCefRenderHandler* rh = new MinetestCefRenderHandler(m_driver, m_texture,
        width, height);
	m_client = new MinetestCefClient(rh);
}

WebPage::WebPage(std::string name, video::IVideoDriver *driver,
		video::IImage *image, int width, int height)
	: m_name(name)
	, m_width(width)
	, m_height(height)
	, m_driver(driver)
	, m_image(image)
{
	CHECK(image);
	MinetestCefRenderHandler* rh = new MinetestCefRenderHandler(m_driver, m_image,
        width, height);
	m_client = new MinetestCefClient(rh);
}

WebPage::WebPage(std::string name, video::IVideoDriver *driver,
		video::ITexture *texture, int width, int height)
	: m_name(name)
	, m_width(width)
	, m_height(height)
	, m_driver(driver)
	, m_texture(texture)
{
	CHECK(texture);
	MinetestCefRenderHandler* rh = new MinetestCefRenderHandler(m_driver, m_texture,
        width, height);
	m_client = new MinetestCefClient(rh);
}

WebPage::~WebPage()
{
	;
}

void WebPage::Open(std::string url)
{
	CefWindowInfo window_info;
	CefBrowserSettings cefBrowserSettings;
	cefBrowserSettings.windowless_frame_rate = 30;

	std::size_t windowHandle = 0;
	window_info.SetAsWindowless(windowHandle, false);

	m_cefbrowser = CefBrowserHost::CreateBrowserSync(window_info, m_client.get(), url, cefBrowserSettings, NULL);
}

void WebPage::Close()
{
	m_cefbrowser->GetHost()->CloseBrowser(false);
	if (m_guiImage != NULL) {
		m_guiImage->remove();
	}
}

void WebPage::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

MinetestCefRenderHandler::MinetestCefRenderHandler(video::IVideoDriver *driver,
        video::ITexture *texture, int width, int height)
	: m_driver(driver)
	, m_texture(texture)
	, m_width(width)
	, m_height(height)
{;}

MinetestCefRenderHandler::MinetestCefRenderHandler(video::IVideoDriver *driver,
        video::IImage *image, int width, int height)
	: m_driver(driver)
	, m_image(image)
	, m_width(width)
	, m_height(height)
{;}

void MinetestCefRenderHandler::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

bool MinetestCefRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
    rect = CefRect(0, 0, m_width, m_height);
    return true;
/*	if (m_texture != NULL)
	{
		m_texture->lock();
		rect = CefRect(0, 0, m_texture->getSize().Width, m_texture->getSize().Height);
		m_texture->unlock();
		return true;
	}
	else if (m_image != NULL)
	{
		m_image->lock();
		rect = CefRect(0, 0, m_image->getDimension().Width, m_image->getDimension().Height);
		m_image->unlock();
		return true;
	}
	return false;*/
}

void MinetestCefRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	// Danger, Will Robinson!
	if (m_texture != NULL)
	{
		irr::u8* data = (irr::u8*) m_texture->lock();
		memcpy(data, buffer, width*height*4);
		m_texture->unlock();
	}
	else if (m_image != NULL)
	{
		irr::u8* data = (irr::u8*) m_image->lock();
		memcpy(data, buffer, width*height*4);
		m_image->unlock();
	}
}

MinetestCefClient::MinetestCefClient(MinetestCefRenderHandler *renderHandler)
	: m_renderHandler(renderHandler)
{;}

CefRefPtr<CefRenderHandler> MinetestCefClient::GetRenderHandler()
{
	return m_renderHandler;
}

/* GARBAGE
MinetestITextureCefRenderHandler::MinetestITextureCefRenderHandler(video::IVideoDriver *driver, video::ITexture *texture)
	: m_driver(driver)
	, m_texture(texture)
{;}

bool MinetestITextureCefRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	if (m_texture != NULL)
	{
		m_texture->lock();
		rect = CefRect(0, 0, m_texture->getSize().Width, m_texture->getSize().Height);
		m_texture->unlock();
		return true;
	}
	return false;
}

void MinetestITextureCefRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	// Danger, Will Robinson!
	if (m_texture != NULL)
	{
		irr::u8* data = (irr::u8*) m_texture->lock();
		memcpy(data, buffer, width*height*4);
		m_texture->unlock();
	}
}

MinetestITextureCefClient::MinetestITextureCefClient(MinetestITextureCefRenderHandler *renderHandler)
	: m_renderHandler(renderHandler)
{;}

CefRefPtr<CefRenderHandler> MinetestITextureCefClient::GetRenderHandler()
{
	return m_renderHandler;
}

MinetestIImageCefRenderHandler::MinetestIImageCefRenderHandler(video::IVideoDriver *driver, video::IImage *image)
	: m_driver(driver)
	, m_image(image)
{;}

bool MinetestIImageCefRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	if (m_image != NULL)
	{
		m_image->lock();
		rect = CefRect(0, 0, m_image->getDimension().Width, m_image->getDimension().Height);
		m_image->unlock();
		return true;
	}
	return false;
}

void MinetestIImageCefRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	// Danger, Will Robinson!
	if (m_image != NULL)
	{
		irr::u8* data = (irr::u8*) m_image->lock();
		memcpy(data, buffer, width*height*4);
		m_image->unlock();
	}
}

MinetestIImageCefClient::MinetestIImageCefClient(MinetestIImageCefRenderHandler *renderHandler)
	: m_renderHandler(renderHandler)
{;}

CefRefPtr<CefRenderHandler> MinetestIImageCefClient::GetRenderHandler()
{
	return m_renderHandler;
}
*/
