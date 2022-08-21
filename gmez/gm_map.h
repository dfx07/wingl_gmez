////////////////////////////////////////////////////////////////////////////////////
// File: gm_map.h   -- Map data game engine                                         
// Copyright (C) 2021-2022 Thuong.NV   Created : 13/08/2022                         
// For conditions of distribution and use, see copyright notice in readme.txt       
////////////////////////////////////////////////////////////////////////////////////

struct cell_map_data
{
    float  x;      //x start top
    float  y;      //y start right

    int    irow;
    int    icol;

    void*  m_data;
};


class gm_map
{
public:
    cell_map_data* m_map_data = NULL;

    // Color cell
    float m_r;
    float m_g; 
    float m_b;

    // Padding cell
    float m_padding = 0.f;

    // Sizeof rows and columns
    int   m_rows;
    int   m_columns;

    float m_xpos;
    float m_ypos;

    // Size view map
    float m_width;
    float m_height;

    float m_cellwidth;
    float m_cellheight;

    bool  m_autofree; // Auto free data for cell

public:
    gm_map() : m_xpos(0.f), m_ypos(0.f),
        m_autofree(false)
    {

    }

    ~gm_map()
    {
        if (!m_map_data) return;

        // Free memory data auto
        if (m_autofree)
        {
            for (int i = 0; i < m_rows * m_columns; i++)
            {
                delete m_map_data[i].m_data;
            }
        }

        delete[] m_map_data;
    }

    void AutoFreeData(bool bAuto = true)
    {
        m_autofree = bAuto;
    }

    // i : row, j columns
    cell_map_data* Get(int i, int j)
    {
        if (!m_map_data) return NULL;
        if (i < 0 || i >= m_rows || j < 0 || j >= m_columns)
            return NULL;

        return &m_map_data[i *m_columns + j];
    }

    int Columns() { return m_columns; }
    int Rows()    { return m_rows;    }

public: // Public function handle check
    bool PointInCell(float x, float y, int i, int j)
    {
        cell_map_data* cell = Get(i, j);

        if (!cell) return false;

        if (x > cell->x && x <= cell->x + m_cellwidth &&
            y > cell->y && y <= cell->y + m_cellheight)
        {
            return true;
        }
        return false;
    }

    cell_map_data* GetCellPoint(float x, float y)
    {
        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_columns; j++)
            {
                if (PointInCell(x, y, i, j))
                {
                    return Get(i, j);
                }
            }
        }
        return NULL;
    }

private:
    void InitMapData()
    {
        m_map_data = new cell_map_data[m_rows * m_columns];

        float xstart, ystart;

        xstart = m_xpos + m_padding;

        for (int i = 0; i < m_rows; i++)
        {
            ystart = m_ypos + m_padding;

            for (int j = 0; j < m_columns; j++)
            {
                cell_map_data* cell = Get(i, j);
                if (cell)
                {
                    cell->m_data = NULL;
                    cell->irow = i;
                    cell->icol = j;
                    cell->x    = xstart;
                    cell->y    = ystart;
                }
                ystart += m_cellheight + m_padding;
            }
            xstart += m_cellwidth + m_padding;
        }
    }

public:

    // Reset data for a cell map
    void FreeCell(int i, int j)
    {
        cell_map_data* cell = Get(i, j);

        if (!cell) return;

        cell->icol = 0;
        cell->icol = 0;


        cell->x    = 0;
        cell->y    = 0;

        if (m_autofree)
        {
            delete cell->m_data;
        }

        cell->m_data = NULL;
    }

    void CreateMapSquare(int nrows)
    {
        // Calculate the necessary statistics before initialization
        m_rows = nrows;

        // Calculate cell width, height
        m_cellwidth  = float(m_width / nrows);
        m_cellwidth -= m_padding;

        m_cellheight = m_cellwidth;
        m_columns    = int(m_height / (m_cellheight + m_padding));

        this->InitMapData();
    }

    void CreateMapCustom(float cellwidth, float cellheight)
    {
        m_cellwidth  = cellwidth;
        m_cellheight = cellheight;

        // Calculate the necessary statistics before initialization
        m_rows    = int(m_width  / (m_cellwidth + m_padding));
        m_columns = int(m_height / (m_cellheight+ m_padding));

        this->InitMapData();
    }

    void ClearMap()
    {
        // Free memory data auto
        if (m_autofree)
        {
            for (int i = 0; i < m_rows * m_columns; i++)
            {
                delete m_map_data[i].m_data;
            }
        }

        m_rows    = 0;
        m_columns = 0;

        delete[] m_map_data;
        m_map_data = NULL;
    }

public:

    void Init(int xpos, int ypos, float width, float height , float padding = 0.f)
    {
        m_xpos = (float)xpos;
        m_ypos = (float)ypos;

        m_padding =  padding;
        m_width   =  width;
        m_height  =  height;
    }

    void SetColor(float r = 1.f, float g = 1.f, float b = 1.f)
    {
        m_r      = r;
        m_g      = g;
        m_b      = b;
    }
};