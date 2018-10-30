#include <pangolin/pangolin.h>
#include <opencv/cv.hpp>

#define READ_BUFFER

int main( int /*argc*/, char** /*argv*/ )
{
    pangolin::CreateWindowAndBind("Main",640,480);
    glEnable(GL_DEPTH_TEST);

    // Define Projection and initial ModelView matrix
    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(640,480,420,420,320,240,0.2,100),
        pangolin::ModelViewLookAt(-2,2,-2, 0,0,0, pangolin::AxisY)
    );

    // Create Interactive View in window
    pangolin::Handler3D handler(s_cam);
    pangolin::View& d_cam = pangolin::CreateDisplay()
            .SetBounds(0.0, 1.0, 0.0, 1.0, -640.0f/480.0f)
            .SetHandler(&handler);

#ifdef READ_BUFFER
    // reserved memory for colour and depth images
    const uint h=480, w=640;
    cv::Mat colour(h, w, CV_8UC3);
    cv::Mat_<float> depth_gl(h, w);
    cv::Mat_<uint8_t> depth_ui8(h, w);
#endif

    while( !pangolin::ShouldQuit() )
    {
        // Clear screen and activate view to render into
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        d_cam.Activate(s_cam);

        // Render OpenGL Cube
        pangolin::glDrawColouredCube();

        // Swap frames and Process Events
        pangolin::FinishFrame();
#ifdef READ_BUFFER
        GLuint query;
        glGenQueries(1, &query);
        glQueryCounter(query, GL_TIMESTAMP);
        int done;

        GLint64 tgl_start, tgl_end;

        // read colour buffer
        // read BGR instead of RGB because of OpenCV's colour channel order
        glGetInteger64v(GL_TIMESTAMP, &tgl_start);
        glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, colour.data);
        if(glGetError()!=GL_NO_ERROR) { throw std::runtime_error("glReadPixels (GL_BGR) failed!"); }
        for(done = 0; !done; glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &done));
        glGetInteger64v(GL_TIMESTAMP, &tgl_end);
        std::cout << "gl colour read time: " << (tgl_end-tgl_start)/float(1e6) << " ms" << std::endl;

        cv::imwrite("colour.png", colour);

        // read depth buffer
        glGetInteger64v(GL_TIMESTAMP, &tgl_start);
        glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depth_gl.data);
        if(glGetError()!=GL_NO_ERROR) { throw std::runtime_error("glReadPixels (GL_DEPTH_COMPONENT) failed!"); }
        for(done = 0; !done; glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &done));
        glGetInteger64v(GL_TIMESTAMP, &tgl_end);
        std::cout << "gl depth read time: " << (tgl_end-tgl_start)/float(1e6) << " ms" << std::endl;

        cv::normalize(depth_gl, depth_ui8, 0, 255, cv::NORM_MINMAX);

        cv::imwrite("depth.png", depth_ui8);
#endif
    }
    
    return 0;
}
