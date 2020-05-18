//#include <glad\glad.h>
//#include <glfw\glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <iostream>
//#include <filesystem>
//
//#include "stb_image.h"
//#include "Shader.h"
//#include "MainAfterTex.h"
//
//int main(int argc, char** argv)
//{
//	int w = 1000;
//	int h = 750;
//	glfwinit();
//	glfwwindowhint(glfw_context_version_major, 3);
//	glfwwindowhint(glfw_context_version_minor, 3);
//	glfwwindowhint(glfw_opengl_profile, glfw_opengl_core_profile);
//
//	glfwwindow* window = glfwcreatewindow(w, h, "particle library", null, null);
//	if (window == null)
//	{
//		std::cout << "failed to create glfw window" << std::endl;
//		glfwterminate();
//		return -1;
//	}
//	glfwmakecontextcurrent(window);
//
//	if (!gladloadglloader((gladloadproc)glfwgetprocaddress))
//	{
//		std::cout << "failed to initialize glad" << std::endl;
//		return -1;
//	}
//
//	glviewport(0, 0, w, h);
//
//	shader shader("vertexshader.txt","fragmentshader.txt");
//
//	float vertices[] = {
//		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
//		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
//		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 	
//	};
//
//	unsigned int vbo, vao;
//	glgenvertexarrays(1, &vao);
//	glgenbuffers(1, &vbo);
//	// bind the vertex array object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glbindvertexarray(vao);
//
//	glbindbuffer(gl_array_buffer, vbo);
//	glbufferdata(gl_array_buffer, sizeof(vertices), vertices, gl_static_draw);
//
//	//pos
//	glvertexattribpointer(0, 3, gl_float, gl_false, 6 * sizeof(float), (void*)0);
//	glenablevertexattribarray(0);
//	//color
//	glvertexattribpointer(1, 3, gl_float, gl_false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glenablevertexattribarray(1);
//	
//	while (!glfwwindowshouldclose(window))
//	{
//		if (glfwgetkey(window, glfw_key_escape) == glfw_press)
//			glfwsetwindowshouldclose(window, true);
//
//		glclearcolor(0.2f, 0.3f, 0.3f, 1.0f);
//		glclear(gl_color_buffer_bit);
//
//		shader.useprogram();
//
//		glbindvertexarray(vao);
//		gldrawarrays(gl_triangles, 0, 3);
//
//		glfwswapbuffers(window);
//		glfwpollevents();
//	}
//
//	gldeletevertexarrays(1, &vao);
//	gldeletebuffers(1, &vbo);
//
//	glfwterminate();
//	return 0;
//}
//
//
