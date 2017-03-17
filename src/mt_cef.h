#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"

#include "irrlicht.h" // createDevice
#include "irrlichttypes_extrabloated.h"

class MinetestCefRenderHandler : public CefRenderHandler
{
private:
	// Minetest core 3D members
    video::IVideoDriver *m_driver;
    video::ITexture *m_texture;

    // CefRenderHandler interface
public:
    MinetestCefRenderHandler(video::IVideoDriver *driver, video::ITexture *texture);

    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);
    
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height);

    // CefBase interface
public:
    IMPLEMENT_REFCOUNTING(MinetestCefRenderHandler);

};

// for manual render handler
class MinetestCefClient : public CefClient
{
private:
    CefRefPtr<CefRenderHandler> m_renderHandler;

public:
    MinetestCefClient(MinetestCefRenderHandler *renderHandler);

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

    IMPLEMENT_REFCOUNTING(MinetestCefClient);
};

class WebPage
{
private:
    video::IVideoDriver *m_driver;
    video::ITexture *m_texture;
	CefRefPtr<MinetestCefClient> m_client;
public:
	WebPage(video::IVideoDriver *driver, video::ITexture *texture);
	~WebPage();
	void Open(std::string url);
	void Close();
	void SetSize(int width, int height);
};

class MinetestBrowser
{
public:
	MinetestBrowser();
	~MinetestBrowser();
	static MinetestBrowser* GetInstance();
	static void Initialize();
	static void Shutdown();
	static void Update();
	WebPage* CreateWebPage(video::IVideoDriver *driver, video::ITexture *texture, int width, int height);
	WebPage* CreateWebPage(video::IVideoDriver *driver, video::ITexture *texture, int width, int height, std::string url);
};
