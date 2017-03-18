#include <unordered_map>

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
	std::string m_name;
    video::IVideoDriver *m_driver;
    gui::IGUIImage* m_guiImage;
    video::ITexture *m_texture;
	CefRefPtr<MinetestCefClient> m_client;
	CefRefPtr<CefBrowser> m_cefbrowser;
public:
	WebPage(std::string name, video::IVideoDriver *driver,
			irr::gui::IGUIImage *image, video::ITexture *texture);
	~WebPage();
	void Open(std::string url);
	void Close();
	void SetSize(int width, int height);
};

class MinetestBrowser
{
private:
	std::unordered_map<std::string, WebPage*> m_webPages;
public:
	MinetestBrowser();
	~MinetestBrowser();
	static MinetestBrowser* GetInstance();
	static bool IsInitialized();
	static void Initialize();
	static void Shutdown();
	static void Update();
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::ITexture *texture, v2s32 pos, v2s32 geom);
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::ITexture *texture, v2s32 pos, v2s32 geom, std::string url);
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom);
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom, std::string url);
	void CloseWebPage(std::string name);
};
