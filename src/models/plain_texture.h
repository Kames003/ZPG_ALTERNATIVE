/**
 * @file plain_texture.h
 *
 * @brief Plain model s UV textúrovými súradnicami
 *
 * @details Command for rendering: glDrawArrays(GL_TRIANGLES, 0, 6);
 *          Formát: position(3) + normal(3) + texCoords(2) = 8 floats per vertex
 *          UV súradnice sú 0-10 pre hustejšiu textúru (10x opakovanie)
 *
 * @year 2025
 **/

const float plain_texture[48] = {
    // position          normal              UV coords
    // Prvý trojuholník
     1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   30.0f, 30.0f,
     1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   30.0f,  0.0f,
    -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,    0.0f,  0.0f,

    // Druhý trojuholník
    -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,    0.0f, 30.0f,  // ľavý horný
     1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   30.0f, 30.0f,  // pravý horný
    -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,    0.0f,  0.0f   // ľavý dolný
};
