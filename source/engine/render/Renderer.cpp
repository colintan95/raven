#include "Renderer.h"

#include "platform/PlatformWindow.h"

Renderer::Renderer(PlatformWindow* platformWindow) {
	m_PlatformWindowPtr = platformWindow;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const char* version = (const char*)glGetString(GL_VERSION);

    LOG_PRINT(version);
}

Renderer::~Renderer() {


	m_PlatformWindowPtr = nullptr;
}

void Renderer::PreRender() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::PostRender() {
	m_PlatformWindowPtr->DrawWindow();
}

void Renderer::Render() {

}

int Renderer::GetWindowWidth() const {
	return m_PlatformWindowPtr->GetWindowWidth();
}

int Renderer::GetWindowHeight() const {
	return m_PlatformWindowPtr->GetWindowHeight();
}
