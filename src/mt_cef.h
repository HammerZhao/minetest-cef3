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
    video::IVideoDriver *m_driver = NULL;
    video::ITexture *m_texture = NULL;
    video::IImage *m_image = NULL;
	int m_width;
	int m_height;

    // CefRenderHandler interface
public:
    MinetestCefRenderHandler(video::IVideoDriver *driver, video::ITexture *texture,
        int width, int height);
    MinetestCefRenderHandler(video::IVideoDriver *driver, video::IImage *image,
        int width, int height);

    void SetSize(int width, int height);

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
	int m_width;
	int m_height;
    video::IVideoDriver *m_driver;
    gui::IGUIImage* m_guiImage;
    video::IImage* m_image;
    video::ITexture *m_texture;
	CefRefPtr<MinetestCefClient> m_client;
	CefRefPtr<CefBrowser> m_cefbrowser;
public:
	WebPage(std::string name, video::IVideoDriver *driver,
        irr::gui::IGUIImage *image, video::ITexture *texture,
        int width, int height);
	WebPage(std::string name, video::IVideoDriver *driver,
        video::IImage *image, int width, int height);
	WebPage(std::string name, video::IVideoDriver *driver,
        video::ITexture *texture, int width, int height);
	~WebPage();
	void Open(std::string url);
	void Close();
	void SetSize(int width, int height);

	void OnMouseMoved(s32 x, s32 y);
	void OnLeftMousePressedDown(s32 x, s32 y);
	void OnLeftMouseLeftUp(s32 x, s32 y);
	void OnLeftMouseDoubleClick(s32 x, s32 y);
    void OnMouseWheel(int x, int y, f32 delta);
	void OnKeyPressed(SEvent::SKeyInput keyinput);
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
						   video::ITexture *texture, v2s32 geom);
    WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::ITexture *texture, v2s32 geom, std::string url);
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::ITexture *texture, core::dimension2d<u32> dimension);
    WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::ITexture *texture, core::dimension2d<u32> dimension,
						   std::string url);

	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::IImage *image, core::dimension2d<u32> dimension);
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   video::IImage *image, core::dimension2d<u32> dimension, std::string url);

	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom);
	WebPage* CreateWebPage(std::string name, video::IVideoDriver *driver,
						   irr::gui::IGUIEnvironment* guiEnv, v2s32 pos, v2s32 geom, std::string url);

    WebPage* GetWebPage(std::string name);
    void CloseWebPage(std::string name);
	void CloseWebPages();
};
