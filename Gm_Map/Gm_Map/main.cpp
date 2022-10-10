#include "sysutils.h"
#include "dbstruct.h"
#include <wglhandle.h>
#include <GLCamera.h>

#include <gm_map.h>
#include <gm_particle.h>

___USELIBFOX____;

GLCamera2D    cam2d;

gm_map        map2d;

cell_map_data* cell_move = NULL;
cell_map_data* cell_move1 = NULL;

vector<sand_piel>  sands;

vector<water_piel> waters;

wall_piel wall(5);

int input_mode = 0;
int count_change = 0;

GLuint vAO_sand;
GLuint vBO_sand;


glm::vec3 pos = { 0, 0, 8 };
glm::vec3 dir = { 0, 0, 1 };
glm::vec3 up  = { 0, 1, 0 };

const int n_inp_size = 5;

int input_shape[n_inp_size][n_inp_size]
= 
{
    {0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0},
};

void CbbChange(Window* win, Combobox* cbb)
{
    input_mode = *(int *)cbb->GetSelectData();
}

std::vector<vec2> cells;

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
    map2d.CreateMapSquare(200);

	for (int i = 0; i < map2d.m_nx; i++)
	{
		for (int j = 0; j < map2d.m_ny; j++)
		{
			auto cell = map2d.Cell(i, j);

			cells.push_back({cell->xpos					  , cell->ypos						});
			cells.push_back({cell->xpos + map2d.m_cellwidth, cell->ypos						});
			cells.push_back({cell->xpos + map2d.m_cellwidth, cell->ypos + map2d.m_cellheight});
			cells.push_back({cell->xpos					  , cell->ypos + map2d.m_cellheight });
		}
		//break;
	}





    Combobox* cbb = new Combobox(20, 20);

    cbb->AddItem(L"sand" , new int(0));
    cbb->AddItem(L"water", new int(1));
    cbb->AddItem(L"wall" , new int(2));

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


    if (cell_move)
    {

        cout << cell_move->x << " : " << cell_move->y << endl;
    }

}

void MouseButton(Window* win, int btn, int action)
{
	if (action == GL_PRESSED)
	{
		if (btn == GLMouse::RightButton)
		{

		}
	}
	if (action == GL_PRESSED)
	{
		if (btn == GLMouse::LeftButton)
		{

		}
	}
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
                for (int i = -n_inp_size / 2; i < n_inp_size / 2; i++)
                {
                    for (int j = -n_inp_size / 2; j < n_inp_size / 2; j++)
                    {
                        cell_map_data* temp =  map2d.Cell(cell->x +i, cell->y +j);

                        if (temp && !temp->m_data)
                        {
                            temp->m_data = new sand_piel(temp->x, temp->y);
                        }
                    }
                }
            }
            else if (input_mode == 1)
            {
                //waters.push_back(water_piel(cell->irow, cell->icol));
                if (cell->m_data == NULL)
                {
                    cell->m_data = new water_piel(cell->x, cell->y);
                }
            }
            else if (input_mode == 2)
            {
                //vector<PIXEL> nei = wall.neighbor_pixel(cell->irow, cell->icol);

                //for (int i = 0; i < nei.size(); i++)
                //{
                //    if (map2d.CellState(nei[i].x, nei[i].y) == 0)
                //    {
                //        wall.Add(nei[i].x, nei[i].y);
                //        cell_map_data* cell = map2d.Get(nei[i].x, nei[i].y);
                //        cell->m_data = &wall;
                //    }
                //}
            }
        }
    }
}

float pixelx, pixely;

float time_update_water = 0;

void OnProcess(Window* win)
{
    int nei[8]; int x, y;

	//glBindVertexArray(vAO_sand);
	//glDrawArrays(GL_QUADS, 0, cells.size());

    //for (int i = 0; i < map2d.m_nx ; i++)
    //{
    //    for (int j = 0; j < map2d.m_ny; j++)
    //    {
    //        cell_map_data* cell = map2d.Cell(i, j);

    //        if (cell->m_data)
    //        {
    //            //switch (cell->m_data->GetType())
    //            //{
    //            //case piel_type::sand:
    //            //{
    //            //    sand_piel* sand = (sand_piel*)cell->m_data;
    //            //    nei[0] = map2d.CellState(i - 1, j - 1);
    //            //    nei[1] = map2d.CellState(i    , j - 1);
    //            //    nei[2] = map2d.CellState(i + 1, j - 1);
    //            //    nei[3] = map2d.CellState(i - 1, j    );
    //            //    nei[4] = map2d.CellState(i + 1, j    );
    //            //    nei[5] = map2d.CellState(i - 1, j + 1);
    //            //    nei[6] = map2d.CellState(i    , j + 1);
    //            //    nei[7] = map2d.CellState(i + 1, j + 1);

    //            //    sand->Update(nei);
    //            //    sand->GetXY(x, y);

    //            //    map2d.MoveCell(i, j, x, y);
    //            //}

    //            //break;
    //            //default:
    //            //    break;
    //            //}
    //        }
    //    }
    //}

    map2d.Commit();
}

void DrawCell(cell_map_data* cell, float widthcell, float heightcell)
{
    if (!cell) return;

    glBegin(GL_QUADS);
    {
        glVertex2f(cell->xpos            , cell->ypos             );
        glVertex2f(cell->xpos + widthcell, cell->ypos             );
        glVertex2f(cell->xpos + widthcell, cell->ypos + heightcell);
        glVertex2f(cell->xpos            , cell->ypos + heightcell);
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
	if (!sand) return;
    int x, y = 0;
    sand->GetXY(x, y);
    cell_map_data* cell = map2d.Cell(x, y);

    if (!cell) return;

    glBegin(GL_QUADS);
    {
        glVertex2f(cell->xpos            , cell->ypos             );
        glVertex2f(cell->xpos + widthcell, cell->ypos             );
        glVertex2f(cell->xpos + widthcell, cell->ypos + heightcell);
        glVertex2f(cell->xpos            , cell->ypos + heightcell);
    }
    glEnd();
}

void DrawWater(water_piel* water, float widthcell, float heightcell)
{
    int x, y = 0;
    water->GetXY(x, y);
    cell_map_data* cell = map2d.Cell(x, y);

    if (!cell) return;
    glColor3f(0.f, 0.f, water->blue);

    glBegin(GL_QUADS);
    {
        glVertex2f(cell->xpos            , cell->ypos             );
        glVertex2f(cell->xpos + widthcell, cell->ypos             );
        glVertex2f(cell->xpos + widthcell, cell->ypos + heightcell);
        glVertex2f(cell->xpos            , cell->ypos + heightcell);
    }
    glEnd();
}

void DrawWallPixel(int x, int y, float widthcell, float heightcell)
{
    cell_map_data* cell = map2d.Cell(x, y);

    if (!cell) return;
    glColor3f(0.6f, 0.1f, 0.1f);

    glBegin(GL_QUADS);
    {
        glVertex2f(cell->xpos            , cell->ypos              );
        glVertex2f(cell->xpos + widthcell, cell->ypos              );
        glVertex2f(cell->xpos + widthcell, cell->ypos + heightcell);
        glVertex2f(cell->xpos            , cell->ypos + heightcell);
    }
    glEnd();
}

void OnDraw(Window* win)
{
	static bool first = false;
	if (!first)
	{
		// Create data opengl sand
		glGenVertexArrays(1, &vAO_sand);
		glGenBuffers(1, &vBO_sand);
		first = true;
	}

		glBindVertexArray(vAO_sand);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vBO_sand);
			glBufferData(GL_ARRAY_BUFFER, cells.size() * sizeof(vec2), &cells[0], GL_STATIC_DRAW);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(vec2), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);


    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam2d.UseMatrix();

	glColor3f(1.0, 1.0, 0);
	glBindVertexArray(vAO_sand);
	glDrawArrays(GL_LINE_STRIP, 0, cells.size());

	//DrawCell(map2d.Cell(0, 0), map2d.m_cellwidth, map2d.m_cellheight);

	//glBegin(GL_QUADS);
	//	glVertex2f(cells[0].x, cells[0].y);
	//	glVertex2f(cells[1].x, cells[1].y);
	//	glVertex2f(cells[2].x, cells[2].y);
	//	glVertex2f(cells[3].x, cells[3].y);
	//glEnd();

   // for (int i = 0; i < map2d.m_nx; i++)
   // {
   //     for (int j = 0; j < map2d.m_ny; j++)
   //     {
   //         cell_map_data* cell = map2d.Cell(i, j);
			//glColor3f(1.f, 0.f, 0.f);
			//DrawCell(cell, map2d.m_cellwidth, map2d.m_cellheight);

   //         cell->state = 0;
   //         
   //         if (cell->m_data)
   //         {
   //             //switch (cell->m_data->GetType())
   //             //{
   //             //    case piel_type::sand:
   //             //    {
   //             //        sand_piel* sand = (sand_piel*)cell->m_data;

   //             //        glColor3f(1.f, 0.f, 0.f);
   //             //        DrawSand(sand, map2d.m_cellwidth, map2d.m_cellheight);
   //             //    }

   //             //    break;
   //             //case piel_type::water:
   //             //    {
   //             //        water_piel* water = (water_piel*)cell->m_data;

   //             //        int sub = rand() % 2;
   //             //        if (sub == 0)
   //             //        {
   //             //            water->blue -= 0.06f;
   //             //        }
   //             //        else {
   //             //            water->blue += 0.06f;
   //             //        }

   //             //        if (water->blue > 1.0f)
   //             //        {
   //             //            water->blue = 0.5f;
   //             //        }
   //             //        if (water->blue < 0.5f)
   //             //        {
   //             //            water->blue = 1.f;
   //             //        }

   //             //        glColor3f(0.f, 0.f, water->blue);
   //             //        DrawWater(water, map2d.m_cellwidth, map2d.m_cellheight);
   //             //    }
   //             //    break;
   //             //default:
   //             //    break;
   //             //}
   //         }
   //     }
   // }
    //Sleep(50);
    //DrawCell(cell_move, map2d.m_cellwidth, map2d.m_cellheight);
    //DrawCell(cell_move1, map2d.m_cellwidth, map2d.m_cellheight);

    //glColor3f(1.0, 0.0, 0.0);
    //if (cell_move)
    //{
    //    DrawCellFull(cell_move, map2d.m_cellwidth, map2d.m_cellheight);
    //}
}

int main()
{
	WndProp adven;
	adven.m_iAntialiasing = 8;

	Window* win = new Window(L"MAP", 0, 0, 640, 480, &adven);
	win->SetOnDrawfunc(OnDraw);
	win->SetOnCreatedfunc(OnCreate);
	win->SetProcessfunc(OnProcess);
	win->SetOnMouseScrollfunc(OnMouseScroll);
	win->SetOnMouseButtonfunc(MouseButton);
	win->SetOnMouseButtonRealtfunc(OnMouseButtonRealtime);
	win->WriteSystemInfo(true);
	win->SetFont(L"Segoe UI", 14);

	win = fox_create_window(win);

	if (win)
	{
		while (!win->closed())
		{
			win->process();
			win->draw();
			win->wait_event();
		}
	}
	fox_destroy_window(win);
}
