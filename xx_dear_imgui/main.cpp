#include <intro.h>
#include <stdint.h>

#ifndef __INTRO__
#include <string.h>
#include <stdlib.h>

#include "../ext/gl.h"
#include <SDL2/SDL.h>

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_opengl3.h"
#include "../ext/imgui/imgui_impl_sdl.h"
#endif

const int window_width = 800;
const int window_height = 600;

typedef enum {
    CHR_KRIS,
    CHR_RALSEI,
    CHR_SUSIE,
    CHR_NOELLE,
    CHR_BIRDLEY,
    CHR_COUNT
} Character;

typedef union {
    struct{ float x,y; };
    float e [2] I(~gui_show);
} Vector2 I(gui_vector);

typedef union {
    struct{ float r, g, b; };
    float e [3] I(remove gui_show);
} Color3f I(gui_edit_color, gui_color {255,0,128,255});

typedef int32_t s32;

typedef struct {
    struct {
        Vector2 position;
        Color3f color;
    } square I(note "data for the square in the background");
    int some_int;
    float some_float;
    double some_double I(gui_scale 0.1, min -34, max 57.0, format "%04.2f");
    Character fav_character I(note "enums become combo boxes when they can");
    struct {
        s32 def I(note "intro understands typedefs to be distinct from their origin");
        uint8_t yuh I(note "note the limits on the slider");
    } sub_data I(note "anon structs work");
    int * ptr_i I(~edit);
} Data;

#ifndef __INTRO__
struct {
    SDL_GLContext sdlc;
    GLint program;
    GLuint vbo;
    GLuint vao;
    struct {
        GLint screen_dimensions;
        GLint color;
    } uniform;
} glc = {0};
#endif

typedef Vector2 Vertex;

typedef int intv8 [8];

typedef struct {
    Vertex * vertices I(length count_vertices);
    int count_vertices I(~gui_edit);
} VertexGroup;

#include "main.cpp.intro"

void setup_gl();

void
sdl_abort() {
    fprintf(stderr, "SDL_Error: %s\n", SDL_GetError());
    exit(1);
}

int
main(int argc, char * argv []) {
    SDL_Window * window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) sdl_abort();

    window = SDL_CreateWindow(
        "intro DealImGui Example",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    if (!window) sdl_abort();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glc.sdlc = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    if (gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress) == 0) {
        fprintf(stderr, "Failed to load GL procedures.\n");
        exit(1);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glc.sdlc);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    setup_gl();

    glClearColor(0.1, 0.1, 0.1, 1);

    ImGuiIO & io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../ext/Cousine-Regular.ttf", 15);

    // looks bad with dpi scaling, i can't find a way to fix it.
    float xdpi, ydpi;
    if (SDL_GetDisplayDPI(0, NULL, &xdpi, &ydpi) == 0) {
        io.DisplayFramebufferScale = ImVec2(xdpi, ydpi);
        fprintf(stderr, "xdpi: %f, ydpi: %f\n", xdpi, ydpi);
    } else {
        fprintf(stderr, "failed to get dpi??\n");
    }

    Data data;
    memset(&data, 0, sizeof(data));
    data.square.position.x = 500;
    data.square.position.y = 200;
    data.square.color = Color3f{1.0, 1.0, 1.0};

    int number = 5;
    data.ptr_i = &number;

    intv8 num_array;
    memset(num_array, 0, sizeof(num_array));

    Vertex square [4] = {
        {-50.0, +50.0},
        {+50.0, +50.0},
        {-50.0, -50.0},
        {+50.0, -50.0},
    };

    bool loop = true;
    while (loop) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);

            switch(e.type) {
            case SDL_QUIT: {
                loop = false;
            }break;

            case SDL_KEYDOWN: {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    loop = false;
                }
            }break;

            default: break;
            }
        }

        VertexGroup v_group;
        v_group.vertices = square;
        v_group.count_vertices = 4;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test window");
        ImGui::Text("Hello there!");
        intro_imgui_edit(&data, ITYPE(Data));
        //intro_imgui_edit(ITYPE(Data), ITYPE(IntroType), "ITYPE(Data)"); // values could be in a write-protected page
        intro_imgui_edit(num_array, ITYPE(intv8));
        intro_imgui_edit(&v_group, ITYPE(VertexGroup));
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT);
        {
            Vertex square_moved [4];
            memcpy(square_moved, square, sizeof(square_moved));
            for (int i=0; i < 4; i++) {
                square_moved[i].x += data.square.position.x;
                square_moved[i].y += data.square.position.y;
            }

            glUniform3fv(glc.uniform.color, 1, data.square.color.e);

            glBindBuffer(GL_ARRAY_BUFFER, glc.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(square), square_moved, GL_STREAM_DRAW);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        SDL_Delay(1);
    }

    SDL_GL_DeleteContext(glc.sdlc);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

static uint32_t
load_shaders(const char * vert_filename, const char * frag_filename) {
	GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);

    size_t vert_code_size, frag_code_size;
	char * vert_code = intro_read_file(vert_filename, &vert_code_size);
	char * frag_code = intro_read_file(frag_filename, &frag_code_size);
    assert(vert_code != NULL);
    assert(frag_code != NULL);

	GLint result = 0, gl_log_len; 

	printf("compiling %s ...\n", vert_filename);
    GLint vsize = (GLint)vert_code_size;
	glShaderSource(vert_id, 1, (const char* const*)&vert_code, &vsize);
	glCompileShader(vert_id);
	glGetShaderiv(vert_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &gl_log_len);
	if (gl_log_len > 0) {
		char * log = (char*) malloc(gl_log_len+1);
		glGetShaderInfoLog(vert_id, gl_log_len, NULL, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}

	printf("compiling %s ...\n", frag_filename);
    GLint fsize = (GLint)frag_code_size;
	glShaderSource(frag_id, 1, (const char* const*)&frag_code, &fsize);
	glCompileShader(frag_id);
	glGetShaderiv(frag_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &gl_log_len);
	if (gl_log_len > 0) {
		char * log = (char*) malloc(gl_log_len+1);
		glGetShaderInfoLog(frag_id, gl_log_len, NULL, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}

	printf("linking shader\n");
	GLuint prog_id = glCreateProgram();
	glAttachShader(prog_id, vert_id);
	glAttachShader(prog_id, frag_id);
	glLinkProgram(prog_id);
	glGetProgramiv(prog_id, GL_LINK_STATUS, &result);
	glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &gl_log_len);
	if (gl_log_len > 0) {
		char * log = (char*) malloc(gl_log_len+1);
		glGetProgramInfoLog(prog_id, gl_log_len, NULL, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}

	glDetachShader(prog_id, vert_id);
	glDetachShader(prog_id, frag_id);
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	free(frag_code);
	free(vert_code);

	if (!result) exit(1);

    // this is consistently the worst part of OpenGL

	return prog_id;
}

void
setup_gl() {
    glc.program = load_shaders("shdr.vert", "shdr.frag");
    glUseProgram(glc.program);

    glc.uniform.screen_dimensions = glGetUniformLocation(glc.program, "screen_dimensions");
    glc.uniform.color = glGetUniformLocation(glc.program, "color");

    glUniform2f(glc.uniform.screen_dimensions, (float)window_width, (float)window_height);
    glUniform3f(glc.uniform.color, 1.0, 1.0, 1.0);

    glGenBuffers(1, &glc.vbo);

    glGenVertexArrays(1, &glc.vao);
    glBindVertexArray(glc.vao);

    glBindBuffer(GL_ARRAY_BUFFER, glc.vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
}
