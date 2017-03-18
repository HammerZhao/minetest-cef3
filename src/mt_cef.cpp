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
bool MinetestBrowser::IsInitialized()
{
	return (m_browser != NULL);
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
//	settings.log_severity = LOGSEVERITY_VERBOSE;
	CefString(&settings.log_file).FromASCII("cef.log");
	CefString(&settings.browser_subprocess_path).FromASCII("./cefclient");
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
	DCHECK(m_browser);
	CefDoMessageLoopWork();
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
										video::ITexture *texture, v2s32 pos, v2s32 geom)
{
	// TODO IMPLEMENT THIS
	return NULL;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
										video::ITexture *texture, v2s32 pos, v2s32 geom,
										std::string url)
{
	// TODO IMPLEMENT THIS
	return NULL;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
										irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom)
{
	DCHECK(m_browser);
	video::ITexture *texture =
		driver->addTexture(core::dimension2d<u32>(geom.X, geom.Y),
						   io::path("browsertexture"),
						   irr::video::ECF_A8R8G8B8);
	irr::gui::IGUIImage* image = guiEnv->addImage(texture, core::position2d<s32>(pos.X, pos.Y));
	WebPage* webPage = new WebPage(name, driver, image, texture);
	webPage->SetSize(geom.X, geom.Y);
	m_webPages[name] = webPage;
	return webPage;
}

WebPage* MinetestBrowser::CreateWebPage(std::string name, video::IVideoDriver *driver,
										irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom, std::string url)
{
	DCHECK(m_browser);
	WebPage* webPage = CreateWebPage(name, driver, guiEnv, pos, geom);
	webPage->Open(url);
	return webPage;
}

void MinetestBrowser::CloseWebPage(std::string name)
{
	DCHECK(m_browser);
	WebPage* webPage = m_webPages[name];
	if (webPage == NULL) return;
	webPage->Close();
	m_webPages.erase(name);
	delete webPage;
}

WebPage::WebPage(std::string name, video::IVideoDriver *driver,
				 irr::gui::IGUIImage *image, video::ITexture *texture)
	: m_name(name)
	, m_driver(driver)
	, m_guiImage(image)
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
	CefWindowInfo window_info;
	CefBrowserSettings cefBrowserSettings;
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

CefRefPtr<CefRenderHandler> MinetestCefClient::GetRenderHandler()
{
	return m_renderHandler;
}

