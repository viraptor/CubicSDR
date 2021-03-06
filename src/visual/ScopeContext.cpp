#include "ScopeContext.h"

#include "ScopeCanvas.h"
#include "ColorTheme.h"

ScopeContext::ScopeContext(ScopeCanvas *canvas, wxGLContext *sharedContext) :
        PrimaryGLContext(canvas, sharedContext) {
    glDisable (GL_CULL_FACE);
    glDisable (GL_DEPTH_TEST);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
}

void ScopeContext::DrawBegin(bool clear) {
    if (clear) {
        glClearColor(ThemeMgr::mgr.currentTheme->scopeBackground.r, ThemeMgr::mgr.currentTheme->scopeBackground.g,
                ThemeMgr::mgr.currentTheme->scopeBackground.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    glDisable (GL_TEXTURE_2D);
}

void ScopeContext::DrawTunerTitles(bool ppmMode) {
    glLoadIdentity();

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    float viewHeight = (float) vp[3];
    float hPos = (float) (13) / viewHeight;

    glColor3f(0.65, 0.65, 0.65);

    GLFont::getFont(GLFont::GLFONT_SIZE12).drawString(ppmMode?"Device PPM":"Frequency", -0.66, -1.0+hPos, 12, GLFont::GLFONT_ALIGN_CENTER, GLFont::GLFONT_ALIGN_CENTER);
    GLFont::getFont(GLFont::GLFONT_SIZE12).drawString("Bandwidth", 0.0, -1.0+hPos, 12, GLFont::GLFONT_ALIGN_CENTER, GLFont::GLFONT_ALIGN_CENTER);
    GLFont::getFont(GLFont::GLFONT_SIZE12).drawString("Center Frequency", 0.66, -1.0+hPos, 12, GLFont::GLFONT_ALIGN_CENTER, GLFont::GLFONT_ALIGN_CENTER);
}

void ScopeContext::DrawDeviceName(std::string deviceName) {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    float viewHeight = (float) vp[3];
    float hPos = (float) (viewHeight - 20) / viewHeight;

    glColor3f(0.65, 0.65, 0.65);
    GLFont::getFont(GLFont::GLFONT_SIZE12).drawString(deviceName.c_str(), 1.0, hPos, 12, GLFont::GLFONT_ALIGN_RIGHT, GLFont::GLFONT_ALIGN_CENTER);
}

void ScopeContext::DrawEnd() {
    glFlush();

    CheckGLError();
}

void ScopeContext::DrawDivider() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin (GL_LINES);
    glVertex2f(0.0, -1.0);
    glVertex2f(0.0, 1.0);
    glEnd();
}
