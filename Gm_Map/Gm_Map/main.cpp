#include <GLWindow.h>
#include <GLCamera.h>

#include <gm_map.h>
#include <gm_particle.h>


GLCamera2D    cam2d;

gm_map        map2d;

cell_map_data* cell_move = NULL;

vector<sand_piel>  sands;
vector<water_piel> waters;

int input_mode = 0;
int count_change = 0;


glm::vec3 pos = { 0, 0, 8 };
glm::vec3 dir = { 0, 0, 1 };
glm::vec3 up  = { 0, 1, 0 };


void CbbChange(Window* win, Combobox* cbb)
{
    input_mode = *(int *)cbb->GetSelectData();
}

void OnCreate(Window* win)
{
    cam2d.InitView(win->GetWidth(), win->GetHeight(), 100.0, -1000);
    cam2d.SetUpCamera(pos, dir, up);
    cam2d.UpdateMatrix();

    int winwid = win->GetWidth();
    int winhei = win->GetHeight();

    map2d.Init(-winwid/2, -winhei/2, (float)winwid, (float)winhei, 0.f);
    //map2d.Init(0, 0, (float)winwid/2, (float)winhei, 0.f);
    //map2d.AutoFreeData();
    map2d.CreateMapSquare(200);


    Combobox* cbb = new Combobox(20, 20);

    cbb->AddItem("sand" , new int(0));
    cbb->AddItem("water", new int(1));

    cbb->SetSelect(0);
    cbb->SetEventSelectedChange(CbbChange);

    win->AddControl(cbb);
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
    //cout << win->GetMouseButtonStatus(LeftButton) << endl;
    //if (win->GetMouseButtonStatus(LeftButton))
    //{
    //    float x, y;
    //    win->GetCursorPos(x, y);
    //    glm::vec2 point = cam2d.PointLocal2Global(glm::vec2(x, y));
    //    cell_map_data* cell = map2d.GetCellPoint(point.x, point.y);

    //    if (cell && input_mode ==1)
    //    {
    //        waters.push_back(water_piel(cell->irow, cell->icol));
    //    }
    //}
}


void OnMouseButtonRealtime(Window* win)
{
    if (win->GetMouseButtonStatus(LeftButton))
    {
        float x, y;
        win->GetCursorPos(x, y);
        glm::vec2 point = cam2d.PointLocal2Global(glm::vec2(x, y));
        cell_map_data* cell = map2d.GetCellPoint(point.x, point.y);

        if (cell)
        {
            if (input_mode == 0)
            {
                sands.push_back(sand_piel(cell->irow, cell->icol));
            }
            else
            {
                waters.push_back(water_piel(cell->irow, cell->icol));
            }
        }
    }
}

void OnProcess(Window* win)
{
    int x, y = 0;
    int nei[8];
    for (int i = 0; i < sands.size(); i++)
    {
        sands[i].GetXY(x, y);
        nei[0] = map2d.CellState(x - 1, y - 1);
        nei[1] = map2d.CellState(x    , y - 1);
        nei[2] = map2d.CellState(x + 1, y - 1);
        nei[3] = map2d.CellState(x - 1, y    );
        nei[4] = map2d.CellState(x + 1, y    );
        nei[5] = map2d.CellState(x - 1, y + 1);
        nei[6] = map2d.CellState(x    , y + 1);
        nei[7] = map2d.CellState(x + 1, y + 1);

        int state = sands[i].Update(nei);

        if (state == 0) // stop
        {
            sands[i].GetXY(x, y);
            cell_map_data* cell = map2d.Get(x, y);

            cell->m_data = &sands[i];
        }
    }

    int xnew, ynew;
    for (int i = 0; i < waters.size(); i++)
    {
        int sub = rand() % 2;
        if (sub == 0)
        {
            waters[i].blue -= 0.06;
        }
        else {
            waters[i].blue += 0.06;
        }

        if (waters[i].blue > 1.0)
        {
            waters[i].blue = 0.5;
        }
        if (waters[i].blue < 0.5)
        {
            waters[i].blue = 1;
        }

        waters[i].GetXY(x, y);
        nei[0] = map2d.CellState(x - 1, y - 1);
        nei[1] = map2d.CellState(x, y - 1);
        nei[2] = map2d.CellState(x + 1, y - 1);
        nei[3] = map2d.CellState(x - 1, y);
        nei[4] = map2d.CellState(x + 1, y);
        nei[5] = map2d.CellState(x - 1, y + 1);
        nei[6] = map2d.CellState(x, y + 1);
        nei[7] = map2d.CellState(x + 1, y + 1);

        int state = waters[i].Update(nei);

        //if (state == 1) // move
        //{
            cell_map_data* cellold = map2d.Get(x, y);
            cellold->m_data = NULL;

            waters[i].GetXY(xnew, ynew);
            cell_map_data* cell    = map2d.Get(xnew, ynew);

            cell->m_data = &waters[i];
        //}
    }
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

void DrawSand(sand_piel* sand, float widthcell, float heightcell)
{
    int x, y = 0;
    sand->GetXY(x, y);
    cell_map_data* cell = map2d.Get(x, y);

    if (!cell) return;

    glBegin(GL_QUADS);
    {
        glVertex2f(cell->x, cell->y);
        glVertex2f(cell->x + widthcell, cell->y);
        glVertex2f(cell->x + widthcell, cell->y + heightcell);
        glVertex2f(cell->x, cell->y + heightcell);
    }
    glEnd();
}

void DrawWater(water_piel* water, float widthcell, float heightcell)
{
    int x, y = 0;
    water->GetXY(x, y);
    cell_map_data* cell = map2d.Get(x, y);

    if (!cell) return;
    glColor3f(0.f, 0.f, water->blue);

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

    //glPointSize(4);
    //glBegin(GL_POINTS);
    //{
    //    glVertex2f(0, 0);
    //}
    //glEnd();

    glColor3f(map2d.m_r, map2d.m_g, map2d.m_b);

    //glBegin(GL_QUADS);
    //{
    //    glVertex2f(0 , 0);
    //    glVertex2f(21, 0);
    //    glVertex2f(21, 21);
    //    glVertex2f(0 , 21);
    //}
    //glEnd();

    //for (int i = 0; i < map2d.m_rows; i++)
    //{
    //    for (int j = 0; j < map2d.m_columns; j++)
    //    {
    //        cell_map_data* cell = map2d.Get(i, j);

    //        //if (j % 2) continue;
    //        if (cell)
    //        {
    //            DrawCell(cell, map2d.m_cellwidth, map2d.m_cellheight);
    //        }
    //    }
    //}

    glColor3f(1.0, 0.0, 0.0);
    for (int i = 0; i < sands.size(); i++)
    {
        DrawSand(&sands[i], map2d.m_cellwidth, map2d.m_cellheight);
    }

    for (int i = 0; i < waters.size(); i++)
    {
        DrawWater(&waters[i], map2d.m_cellwidth, map2d.m_cellheight);
    }

    //glColor3f(1.0, 0.0, 0.0);
    //if (cell_move)
    //{
    //    DrawCellFull(cell_move, map2d.m_cellwidth, map2d.m_cellheight);
    //}
}

int main()
{
    GLWindow window;

    WndProp adven;
    adven.m_iAntialiasing = 8;

    Window* win = new Window("MAP", 500, 100, 900, 580);
    win->SetupAdvanced(adven);
    win->WriteSystemInfo(true);

    win->SetProcessfunc(OnProcess);
    win->SetOnDrawfunc(OnDraw);
    win->SetOnCreatedfunc(OnCreate);
    win->SetOnDestroyfunc(OnDestroy);
    win->SetOnKeyboardfunc(Onkeyboard);
    win->SetOnMouseScrollfunc(OnMouseScroll);
    win->SetOnMouseButtonRealtfunc(OnMouseButtonRealtime);
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
