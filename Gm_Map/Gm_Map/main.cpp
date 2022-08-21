#include <GLWindow.h>
#include <GLCamera.h>

#include <gm_map.h>


GLCamera2D    cam2d;

gm_map        map2d;

cell_map_data* cell_move = NULL;


glm::vec3 pos = { 0, 0, 8 };
glm::vec3 dir = { 0, 0, 1 };
glm::vec3 up  = { 0, 1, 0 };

void OnCreate(Window* win)
{
    cam2d.InitView(win->GetWidth(), win->GetHeight(), 100.0, -1000);
    cam2d.SetUpCamera(pos, dir, up);
    cam2d.UpdateMatrix();

    int winwid = win->GetWidth();
    int winhei = win->GetHeight();

    map2d.Init(-winwid/2, -winhei/2, (float)winwid, (float)winhei, 1.f);
    //map2d.Init(0, 0, (float)winwid/2, (float)winhei, 0.f);
    map2d.AutoFreeData();
    map2d.CreateMapSquare(100);

}

void OnDestroy(Window* win)
{
}


void Onkeyboard(Window* win)
{
    if (win->GetKeyboardStatus(KeyA))
    {
    }
    else if (win->GetKeyboardStatus(KeyD))
    {
    }
}

void OnMouseScroll(Window* win)
{
    int delta = win->GetMouseScroll();

    float x, y;
    win->GetCursorPos(x, y);
    cam2d.ZoomTo(x, y, (float)delta);
    cam2d.UpdateMatrix();

}

void OnResize(Window* win)
{
    cam2d.SetViewSize(win->GetWidth(), win->GetHeight());
    cam2d.UpdateMatrix();
}

void OnMouseMove(Window* win)
{
    float x, y;
    win->GetCursorPos(x, y);

    glm::vec2 point = cam2d.PointLocal2Global(glm::vec2(x, y));

    cell_move =  map2d.GetCellPoint(point.x, point.y);

    cout << point.x << " : " << point.y << endl;
}

void OnMouseButton(Window* win)
{

}


void OnProcess(Window* win)
{

}

void DrawCell(cell_map_data* cell, float widthcell, float heightcell)
{
    glBegin(GL_LINE_LOOP);
    {
        glVertex2f(cell->x            , cell->y             );
        glVertex2f(cell->x + widthcell, cell->y             );
        glVertex2f(cell->x + widthcell, cell->y + heightcell);
        glVertex2f(cell->x            , cell->y + heightcell);
    }
    glEnd();
}

void DrawCellFull(cell_map_data* cell, float widthcell, float heightcell)
{
    glBegin(GL_QUADS);
    {
        glVertex2f(cell->x, cell->y);
        glVertex2f(cell->x + widthcell, cell->y);
        glVertex2f(cell->x + widthcell, cell->y + heightcell);
        glVertex2f(cell->x, cell->y + heightcell);
    }
    glEnd();
}

void OnDraw(Window* win)
{
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam2d.UseMatrix();

    glPointSize(4);
    glBegin(GL_POINTS);
    {
        glVertex2f(0, 0);
    }
    glEnd();

    glColor3f(map2d.m_r, map2d.m_g, map2d.m_b);

    //glBegin(GL_QUADS);
    //{
    //    glVertex2f(0 , 0);
    //    glVertex2f(21, 0);
    //    glVertex2f(21, 21);
    //    glVertex2f(0 , 21);
    //}
    //glEnd();

    for (int i = 0; i < map2d.m_rows; i++)
    {
        for (int j = 0; j < map2d.m_columns; j++)
        {
            cell_map_data* cell = map2d.Get(i, j);

            //if (j % 2) continue;
            if (cell)
            {
                DrawCell(cell, map2d.m_cellwidth, map2d.m_cellheight);
            }
        }
    }

    glColor3f(1.0, 0.0, 0.0);
    if (cell_move)
    {
        DrawCellFull(cell_move, map2d.m_cellwidth, map2d.m_cellheight);
    }
}

int main()
{
    GLWindow window;

    WndProp adven;
    adven.m_iAntialiasing = 4;

    Window* win = new Window("MAP", 500, 100, 900, 580);
    win->SetupAdvanced(adven);
    win->WriteSystemInfo(true);

    win->SetProcessfunc(OnProcess);
    win->SetOnDrawfunc(OnDraw);
    win->SetOnCreatedfunc(OnCreate);
    win->SetOnDestroyfunc(OnDestroy);
    win->SetOnKeyboardfunc(Onkeyboard);
    win->SetOnMouseScrollfunc(OnMouseScroll);
    win->SetOnMouseMovefunc(OnMouseMove);
    win->SetOnResizefunc(OnResize);
    win->SetOnMouseButtonfunc(OnMouseButton);

    window.AddWindow(win);

    while (window.Closed())
    {
        window.Process();
        window.Draw();
        window.PollEvent();
    }
}
