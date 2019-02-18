// generated by Fast Light User Interface Designer (fluid) version 1.0010

#include "debuggingWindow.h"

inline void DebuggingWindow::cb_Normal_i(Fl_Menu_*, void*) {
    m_debuggingView->setShadingMode(DebuggingView::NORMAL);
}
void DebuggingWindow::cb_Normal(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Normal_i(o, v);
}

inline void DebuggingWindow::cb_Flat_i(Fl_Menu_*, void*) {
    m_debuggingView->setShadingMode(DebuggingView::FLAT_SHADING);
}
void DebuggingWindow::cb_Flat(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Flat_i(o, v);
}

inline void DebuggingWindow::cb_Wireframe_i(Fl_Menu_*, void*) {
    m_debuggingView->setShadingMode(DebuggingView::WIREFRAME);
}
void DebuggingWindow::cb_Wireframe(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Wireframe_i(o, v);
}

inline void DebuggingWindow::cb_High_i(Fl_Menu_*, void*) {
    m_debuggingView->setQuality(DebuggingView::HIGH_QUALITY);
}
void DebuggingWindow::cb_High(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_High_i(o, v);
}

inline void DebuggingWindow::cb_Medium_i(Fl_Menu_*, void*) {
    m_debuggingView->setQuality(DebuggingView::MEDIUM_QUALITY);
}
void DebuggingWindow::cb_Medium(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Medium_i(o, v);
}

inline void DebuggingWindow::cb_Low_i(Fl_Menu_*, void*) {
    m_debuggingView->setQuality(DebuggingView::LOW_QUALITY);
}
void DebuggingWindow::cb_Low(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Low_i(o, v);
}

inline void DebuggingWindow::cb_Poor_i(Fl_Menu_*, void*) {
    m_debuggingView->setQuality(DebuggingView::POOR_QUALITY);
}
void DebuggingWindow::cb_Poor(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Poor_i(o, v);
}

inline void DebuggingWindow::cb_Default_i(Fl_Menu_*, void*) {
    m_debuggingView->setLighting(DebuggingView::DEFAULT_LIGHTING);
}
void DebuggingWindow::cb_Default(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Default_i(o, v);
}

inline void DebuggingWindow::cb_Scene_i(Fl_Menu_*, void*) {
    m_debuggingView->setLighting(DebuggingView::SCENE_LIGHTING);
}
void DebuggingWindow::cb_Scene(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Scene_i(o, v);
}

inline void DebuggingWindow::cb_Use_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setUseSceneMaterials(m[16].value() != 0);
}
void DebuggingWindow::cb_Use(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Use_i(o, v);
}

inline void DebuggingWindow::cb_Geometry_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowGeometry(m[19].value() != 0);
}
void DebuggingWindow::cb_Geometry(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Geometry_i(o, v);
}

inline void DebuggingWindow::cb_Lights_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowLights(m[20].value() != 0);
}
void DebuggingWindow::cb_Lights(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Lights_i(o, v);
}

inline void DebuggingWindow::cb_Camera_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowCamera(m[21].value() != 0);
}
void DebuggingWindow::cb_Camera(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Camera_i(o, v);
}

inline void DebuggingWindow::cb_Axes_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowAxes(m[22].value() != 0);
}
void DebuggingWindow::cb_Axes(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Axes_i(o, v);
}

inline void DebuggingWindow::cb_Normals_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowNormals(m[23].value() != 0);
}
void DebuggingWindow::cb_Normals(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Normals_i(o, v);
}

inline void DebuggingWindow::cb_Visibility_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowVisibilityRays(m[25].value() != 0);
}
void DebuggingWindow::cb_Visibility(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Visibility_i(o, v);
}

inline void DebuggingWindow::cb_Reflection_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowReflectionRays(m[26].value() != 0);
}
void DebuggingWindow::cb_Reflection(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Reflection_i(o, v);
}

inline void DebuggingWindow::cb_Refraction_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowRefractionRays(m[27].value() != 0);
}
void DebuggingWindow::cb_Refraction(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Refraction_i(o, v);
}

inline void DebuggingWindow::cb_Shadow_i(Fl_Menu_*, void*) {
    const Fl_Menu_Item* m = m_debuggingMenuBar->menu();
    m_debuggingView->setShowShadowRays(m[28].value() != 0);
}
void DebuggingWindow::cb_Shadow(Fl_Menu_* o, void* v) {
    ((DebuggingWindow*)(o->parent()->user_data()))->cb_Shadow_i(o, v);
}

Fl_Menu_Item DebuggingWindow::menu_m_debuggingMenuBar[] = {
    {"Shading", 0, 0, 0, 64, 0, 0, 14, 0},
    {"Normal", 0, (Fl_Callback*)DebuggingWindow::cb_Normal, 0, 12, 0, 0, 14, 0},
    {"Flat shading", 0, (Fl_Callback*)DebuggingWindow::cb_Flat, 0, 8, 0, 0, 14,
     0},
    {"Wireframe", 0, (Fl_Callback*)DebuggingWindow::cb_Wireframe, 0, 136, 0, 0,
     14, 0},
    {"Quality", 0, 0, 0, 64, 0, 0, 14, 0},
    {"High", 0, (Fl_Callback*)DebuggingWindow::cb_High, 0, 8, 0, 0, 14, 0},
    {"Medium", 0, (Fl_Callback*)DebuggingWindow::cb_Medium, 0, 12, 0, 0, 14, 0},
    {"Low", 0, (Fl_Callback*)DebuggingWindow::cb_Low, 0, 8, 0, 0, 14, 0},
    {"Poor", 0, (Fl_Callback*)DebuggingWindow::cb_Poor, 0, 8, 0, 0, 14, 0},
    {0},
    {0},
    {"Lighting", 0, 0, 0, 64, 0, 0, 14, 0},
    {"Default lighting", 0, (Fl_Callback*)DebuggingWindow::cb_Default, 0, 12, 0,
     0, 14, 0},
    {"Scene lighting", 0, (Fl_Callback*)DebuggingWindow::cb_Scene, 0, 8, 0, 0,
     14, 0},
    {0},
    {"Materials", 0, 0, 0, 64, 0, 0, 14, 0},
    {"Use scene materials", 0, (Fl_Callback*)DebuggingWindow::cb_Use, 0, 6, 0,
     0, 14, 0},
    {0},
    {"Display", 0, 0, 0, 64, 0, 0, 14, 0},
    {"Geometry", 0, (Fl_Callback*)DebuggingWindow::cb_Geometry, 0, 6, 0, 0, 14,
     0},
    {"Lights", 0, (Fl_Callback*)DebuggingWindow::cb_Lights, 0, 6, 0, 0, 14, 0},
    {"Camera", 0, (Fl_Callback*)DebuggingWindow::cb_Camera, 0, 6, 0, 0, 14, 0},
    {"Axes", 0, (Fl_Callback*)DebuggingWindow::cb_Axes, 0, 6, 0, 0, 14, 0},
    {"Normals", 0, (Fl_Callback*)DebuggingWindow::cb_Normals, 0, 6, 0, 0, 14,
     0},
    {"Rays", 0, 0, 0, 64, 0, 0, 14, 0},
    {"Visibility", 0, (Fl_Callback*)DebuggingWindow::cb_Visibility, 0, 6, 0, 0,
     14, 0},
    {"Reflection", 0, (Fl_Callback*)DebuggingWindow::cb_Reflection, 0, 6, 0, 0,
     14, 0},
    {"Refraction", 0, (Fl_Callback*)DebuggingWindow::cb_Refraction, 0, 6, 0, 0,
     14, 0},
    {"Shadow", 0, (Fl_Callback*)DebuggingWindow::cb_Shadow, 0, 2, 0, 0, 14, 0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0}};

DebuggingWindow::DebuggingWindow() {
    Fl_Window* w;
    {
        Fl_Window* o = m_debuggingWindow =
            new Fl_Window(386, 395, "Debugging View");
        w = o;
        o->user_data((void*)(this));
        {
            Fl_Menu_Bar* o = m_debuggingMenuBar =
                new Fl_Menu_Bar(0, 0, 390, 25);
            o->box(FL_NO_BOX);
            o->menu(menu_m_debuggingMenuBar);
        }
        o->begin();
        {
            DebuggingView* o = m_debuggingView =
                new DebuggingView(0, 25, 385, 370, "");
            Fl_Group::current()->resizable(o);
        }
        o->end();
    }
}

void DebuggingWindow::show() {
    m_debuggingWindow->show();
    m_debuggingView->show();
}

void DebuggingWindow::hide() {
    m_debuggingView->hide();
    m_debuggingWindow->hide();
}

void DebuggingWindow::redraw() { m_debuggingView->redraw(); }
