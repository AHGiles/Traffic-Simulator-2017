#include "OpenGLHelper.h"

//#define DEBUG

void openGLErrorCheck(char* fileName, int lineNumber) {

#ifdef DEBUG

	bool fatal = false;

	for (GLenum errorCode; (errorCode = glGetError()) != GL_NO_ERROR;) {

		switch (errorCode)
		{
		case 0x0500:
			std::cerr << "An OpenGL function has been given an Invalid Enum on line " << lineNumber << " in " << fileName << std::endl;
			fatal = true;
			break;

		case 0x0501:
			std::cerr << "An OpenGL function has been given an Invalid Value on line " << lineNumber << " in " << fileName << std::endl;
			fatal = true;
			break;

		case 0x0502:
			std::cerr << "An OpenGL function has detected an Invalid Operation on line " << lineNumber << " in " << fileName << std::endl;
			//fatal = true;
			break;

		case 0x0503:
			std::cerr << "An OpenGL function can not completed because of a Stack Overflow at line " << lineNumber << " in " << fileName << std::endl;
			fatal = true;
			break;

		case 0x0504:
			std::cerr << "An OpenGL function can not completed because of a Stack Underflow at line " << lineNumber << " in " << fileName << std::endl;
			fatal = true;
			break;

		case 0x0505:
			std::cerr << "An OpenGL function can not completed because the program is Out of Memory at line " << lineNumber << " in " << fileName << std::endl;
			break;

		case 0x0506:
			std::cerr << "An OpenGL Framebuffer Read/Write cannot complete because the Framebuffer is incomplete at line " << lineNumber << " in " << fileName << std::endl;
			break;

		case 0x0507:
			std::cerr << "The current OpenGL Context has been lost at line " << lineNumber << " in " << fileName << std::endl;
			fatal = true;
			break;

		default:
			std::cerr << "An Unknown Error has occured in OpenGL at line " << lineNumber << " in " << fileName << std::endl;
			break;
		}

		if (fatal) {
			std::cout << "Aborting..." << std::endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
		else {
			std::cout << "Attempting to continue..." << std::endl;
			system("pause");
		}
	}


#endif // DEBUG

}
