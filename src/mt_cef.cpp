#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"

#include "mt_cef.h"

#include "irrlicht.h" // createDevice
#include "irrlichttypes_extrabloated.h"

MinetestBrowser::MinetestBrowser() {
	;
}

MinetestBrowser::~MinetestBrowser() {
	;
}

MinetestBrowser* m_browser = NULL;

// static
MinetestBrowser* MinetestBrowser::GetInstance()
{
	DCHECK(m_browser);
	return m_browser;
}

// static
void MinetestBrowser::Initialize()
{
	DCHECK(!m_browser);
	m_browser = new MinetestBrowser();
	CefMainArgs args;
	CefSettings settings;

	settings.no_sandbox = true;
	settings.single_process = false;
	settings.multi_threaded_message_loop = false;
	settings.windowless_rendering_enabled = true;
	settings.command_line_args_disabled = true;
	settings.log_severity = LOGSEVERITY_VERBOSE;
	CefString(&settings.log_file).FromASCII("cef.log");
	CefString(&settings.browser_subprocess_path).FromASCII("./cefclient");
	CefInitialize(args, settings, nullptr, nullptr);

//	CefInitialize(cefArgs, cefSettings, cefApp.get(), nullptr);
	
/*	CefWindowInfo window_info;
	CefBrowserSettings browserSettings;
	std::size_t windowHandle = 0;
	window_info.SetAsWindowless(windowHandle, false);
	browser = CefBrowserHost::CreateBrowserSync(window_info, cefHandler.get(),
		"http://deanm.github.io/pre3d/monster.html", browserSettings, nullptr);
*/		
//	CefDoMessageLoopWork();

}

// static
void MinetestBrowser::Shutdown()
{
	CefShutdown();
}

void MinetestBrowser::Update()
{
	DCHECK(m_browser);
	CefDoMessageLoopWork();
}

WebPage* MinetestBrowser::CreateWebPage(video::IVideoDriver *driver, video::ITexture *texture, int width, int height) {
	DCHECK(m_browser);
	WebPage* webPage = new WebPage(driver, texture);
	webPage->SetSize(width, height);
	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(video::IVideoDriver *driver, video::ITexture *texture, int width, int height, std::string url) {
	DCHECK(m_browser);
	WebPage* webPage = CreateWebPage(driver, texture, width, height);
	webPage->Open(url);
	return webPage;
}

WebPage::WebPage(video::IVideoDriver *driver, video::ITexture *texture)
	: m_driver(driver)
	, m_texture(texture)
{
	MinetestCefRenderHandler* rh = new MinetestCefRenderHandler(m_driver, m_texture);
	m_client = new MinetestCefClient(rh);
}

WebPage::~WebPage()
{
	;
}

void WebPage::Open(std::string url)
{
	CefRefPtr<CefBrowser> browser;
	CefWindowInfo window_info;
	CefBrowserSettings browserSettings;
	std::size_t windowHandle = 0;
	window_info.SetAsWindowless(windowHandle, false);
	browser = CefBrowserHost::CreateBrowserSync(window_info, m_client.get(), url, browserSettings, NULL);
}

void WebPage::Close()
{
	;
}

void WebPage::SetSize(int width, int height)
{
	;
}

MinetestCefRenderHandler::MinetestCefRenderHandler(video::IVideoDriver *driver, video::ITexture *texture)
	: m_driver(driver)
	, m_texture(texture)
{;}

bool MinetestCefRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, m_texture->getSize().Width, m_texture->getSize().Height);
	return true;
}

void MinetestCefRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	irr::u8* data = (irr::u8*) m_texture->lock();
	memcpy(data, buffer, width*height*4);
	m_texture->unlock();
}

MinetestCefClient::MinetestCefClient(MinetestCefRenderHandler *renderHandler)
	: m_renderHandler(renderHandler)
{;}

CefRefPtr<CefRenderHandler> MinetestCefClient::GetRenderHandler() {
	return m_renderHandler;
}


