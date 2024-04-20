/*
CHATGPT was used in the making of this program
*/

#include "CControler.h"
#include "PPong.h"
#include <opencv2/highgui.hpp>
#include "cvui.h"
//#include "CSketch.h"

#define CANVAS_NAME "LAB 5 - PONG"

/*Pong class starts here*/
PPong::PPong(cv::Size canvasSize, int comPort) {

    _canvas = cv::Mat::zeros(canvasSize.width, canvasSize.height, CV_8UC3);
    _control.init_com(comPort); // we need to use from CBase!!!
    //_control();
    cvui::init(CANVAS_NAME);
    matSize = _canvas.size();

    _ballPos = cv::Point2f(matSize.width / 2, matSize.height / 2); ///< Stores the current position on the canvas for drawing.
    aiPaddlePos = cv::Point2f(matSize.width, matSize.height / 2);;
    playerPaddlePos = cv::Point2f(0, matSize.height / 2);
    int randDirection = (std::rand() % 2 == 1) ? -1 : 1;
    _ballVel = cv::Point2f(10 * randDirection, std::rand() % 10);
    //_ballVel = cv::Point2f(1, 0);
    aiPaddleVel = cv::Point2f(0, 0);
    rectVelocity = cv::Point2f(0, 0);
    //cv::Point2f ;
    sliderVelocity = 100;
    sliderRadius = 5;

    playerScore = 0;
    enemyScore = 0;

    paddleWidth = matSize.width / 35;
    paddleHeight = matSize.height / 5;

    calc_start = std::chrono::steady_clock::now();
    calc_end = calc_start;
    calc_collisionTime = calc_end;
    deltaT = std::chrono::milliseconds(1);
    //std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start);
    gameOver = " ";
    gamePause = false;
    key = ' ';
    _thread_exit = false;
    isThreading = false;

}
void PPong::resetGame() {

    matSize = _canvas.size();
    _ballPos = cv::Point2f(matSize.width / 2, matSize.height / 2); ///< Stores the current position on the canvas for drawing.
    aiPaddlePos = cv::Point2f(matSize.width, matSize.height / 2);;
    playerPaddlePos = cv::Point2f(0, matSize.height / 2);
    int randDirection = (std::rand() % 2 == 1) ? -1 : 1;
    _ballVel = cv::Point2f(10 * randDirection, std::rand() % 10);
    aiPaddleVel = cv::Point2f(0, 0);
    rectVelocity = cv::Point2f(0, 0);

    playerScore = 0;
    enemyScore = 0;

    calc_start = std::chrono::steady_clock::now();
    calc_end = calc_start;
    calc_collisionTime = calc_end;
    deltaT = std::chrono::milliseconds(1);

    gamePause = true;

}
/*RETURN TRUE means its not on boundary, return false if doesn't change*/
bool PPong::pointWallDetection(cv::Point2f& positionVector) {
    cv::Point2f initialPosition = positionVector;
    positionVector.x = (positionVector.x < 0) ? 0 : positionVector.x;
    positionVector.x = (positionVector.x >= matSize.width) ? matSize.width - 1 : positionVector.x;

    positionVector.y = (positionVector.y < 0) ? 0 : positionVector.y;
    positionVector.y = (positionVector.y >= matSize.height) ? matSize.height - 1 : positionVector.y;

    if ((positionVector.x <= 0 && positionVector.x >= matSize.width - 1) || positionVector.y <= 0 && positionVector.y <= matSize.height-1) {
        return false;
    }
    return true;
}
bool PPong::addYposition(cv::Point2f& positionVector, cv::Point2f& velocityVector) {
    //if (velocityVector.x >= -2 && velocityVector.x <= 2) velocityVector.x = 0;
    //std::cout << velocityVector.y << "velocity vec \n";
    if (velocityVector.y >= -3 && velocityVector.y <= 3) velocityVector.y = 0;
    //positionVector.x = (positionVector.x + velocityVector.x);
    positionVector.y = (positionVector.y - velocityVector.y);
    if (positionVector.y < 0) {
        positionVector.y = 0;
        return false;
    }
    else if(positionVector.y > matSize.height){
        positionVector.y = matSize.height;
        return false;
    }

    return true;
}
void PPong::createControlPaddle(cv::Point2f &position, cv::Point2f& velocity,int paddleWidth, int paddleHeight) {

    cv::Point2f halfHeight = cv::Point2f(0, (paddleHeight / 2));
    cv::Point2f botPos = position + halfHeight;
    cv::Point2f topPos = position - halfHeight;

    bool isBotCollision1 = addYposition(botPos, velocity);
    bool isBotCollision2 = addYposition(topPos, velocity);
    bool isBotCollision3 = addYposition(position, velocity);

    if (!isBotCollision1) {//top collision
        position = cv::Point2f(position.x, matSize.height - halfHeight.y);
        botPos = cv::Point2f(position.x, position.y + halfHeight.y);
        topPos = cv::Point2f(position.x, position.y - halfHeight.y);
    }else if (!isBotCollision2)
    {
        position = cv::Point2f(position.x, halfHeight.y);
        botPos = cv::Point2f(position.x, position.y + halfHeight.y);
        topPos = cv::Point2f(position.x, position.y - halfHeight.y);
    }
}

float PPong::magnitude(const cv::Point2f& point) {
    return std::sqrt(point.x * point.x + point.y * point.y);
}

void PPong::drawGUI() {
    float fps = 1000.0f / deltaT.count(); // Convert ms to seconds, then take the reciprocal for FPS
    //std::cout <<"fps : "<<fps << "\n";
    // GUI FOR BUTTONS
    cv::Mat frame = _canvas.clone();
    cv::Point2f gui_position = cv::Point2f(20, 20);

    if (gameOver != " ") {
        cv::putText(frame, gameOver, cv::Point(matSize.width / 3, matSize.height / 2.5),
            cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 3);
        if (cvui::button(frame, matSize.width / 2.6 + 40, matSize.height / 3 - 70 , "Play again")) {
            gamePause = false;
            gameOver = " ";
        }
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << fps*2;
    std::string fpsString = "FPS: " + stream.str();
    cvui::text(frame, gui_position.x, gui_position.y, fpsString);
    gui_position += cv::Point2f(0, 30);

    cvui::text(frame, matSize.width - 150, gui_position.y, "Enemy Score: " + std::to_string(enemyScore));//alternated
    cvui::text(frame, gui_position.x, gui_position.y, "Player Score: " + std::to_string(playerScore));

    gui_position += cv::Point2f(0, 15);
    cvui::text(frame, gui_position.x, matSize.height - 130, "velocity: ");
    cvui::text(frame, gui_position.x + 150, matSize.height - 130, "radius: ");
    gui_position += cv::Point2f(0, 15);
    cvui::trackbar(frame, gui_position.x, matSize.height - 100, 120, &sliderVelocity, 100, 400);
    cvui::trackbar(frame, gui_position.x+ 150, matSize.height - 100, 120, &sliderRadius, 5, 100);
    if ((cvui::button(frame, gui_position.x, matSize.height - 50, 100, 30, "EXIT") || key == 'q' )) {//  || key == 'Q'
        char key = 'q';
        end();
    }
    gui_position += cv::Point2f(0, 50);

    if ((cvui::button(frame, gui_position.x + 150, matSize.height - 50, 100, 30, "Reset"))) {
        //to be written!
        resetGame();
        //gamePause = true;
        //Sleep(1000);
        gameOver = " ";
        gamePause = false;
    }
    gui_position += cv::Point2f(0, 30);
    frame.copyTo(_canvas);
}

void PPong::update() {

    //auto Fps30 = std::chrono::steady_clock::now();
    if(gamePause == true){
        return;
    }
    std::cout << "update:\n";
    cv::Point xyJoystick;
    _control.analogInputs(xyJoystick);
    rectVelocity = cv::Point2f(0, xyJoystick.y - 50) /2;

    createControlPaddle(playerPaddlePos, rectVelocity,paddleWidth,paddleHeight);
    aiPaddlePos.y = _ballPos.y;

    createControlPaddle(aiPaddlePos, aiPaddleVel,paddleWidth, paddleHeight);
    createBall();

    //std::this_thread::sleep_until(Fps30 + std::chrono::milliseconds(30));
}
void PPong::draw_thread(PPong* ptr) {
    while (!ptr->_thread_exit) {
        ptr->draw();
        // No need to spawn a new thread here, just execute update
    }
}
/*Threading things*/
void PPong::update_thread(PPong* ptr) {
    std::cout<<"thread exit" << !ptr->_thread_exit << "gamePause" << !ptr->gamePause << "\n";
    while (!ptr->_thread_exit) {
        if (!ptr->gamePause) {
            ptr->update();
        }
        // No need to spawn a new thread here, just execute update
    }
}

void PPong::start()
{

    isThreading = true;

    t1 = std::thread(&PPong::update_thread, this);
    t2 = std::thread(&PPong::draw_thread, this);

    //std::thread t1(&PPong::_thread, this);
    t1.detach();
    t2.detach();
    /*
    while (!_thread_exit) {
        this->draw();
    }
    */

    while (key != 'q') {
        key = cv::waitKey(1);
    }
    end();
   // _thread_exit = true;
}
void PPong::end() {
    if (t1.joinable()) {
        t1.join();
    }
    if (t2.joinable()) {
        t2.join();
    }
    _thread_exit = true;
    exit(0);
}
/**
 * @brief Renders the current game state to the canvas.
 *
 * This method should be overridden in derived classes to implement game-specific drawing logic.
 * It updates the canvas with the current game visuals and displays it using OpenCV functions.
 */
void PPong::draw() {
  //  pongMutex.lock
    std::cout << "drawing\n";
    _canvas.setTo(cv::Scalar(0, 0, 0));
    if(isThreading == false){
        key = cv::waitKey(1);// only if multithreading is turned off.
    }
    drawGUI();

    cv::circle(_canvas, _ballPos , sliderRadius, cv::Scalar(255, 255, 255), cv::FILLED);

    createRectangle(playerPaddlePos, paddleWidth, paddleHeight);
    createRectangle(aiPaddlePos, paddleWidth, paddleHeight);

    //calc_start = std::chrono::steady_clock::now
    cvui::update();
    cv::imshow(CANVAS_NAME, _canvas);
    std::this_thread::sleep_until(calc_start + std::chrono::milliseconds(30));
    calc_end = std::chrono::steady_clock::now();
    deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start) * 2;
    calc_start = std::chrono::steady_clock::now();

}
bool PPong::createRectangle(cv::Point2f& centerPoint2f, int rectWidth, int rectHeight ) {
    // Define the size of your rectangle

    // Define the rectangle's top-left and bottom-right corners
    int halfWidth = rectWidth / 2;
    int halfHeight = rectHeight / 2;

    cv::Scalar rectColor(0, 0, 255); // Red color

    cv::Point2f topLeft = cv::Point2f(centerPoint2f.x - halfWidth, centerPoint2f.y - halfHeight);
    cv::Point2f topRight = cv::Point2f(centerPoint2f.x + halfWidth, centerPoint2f.y - halfHeight);
    cv::Point2f bottomLeft = cv::Point2f(centerPoint2f.x - halfWidth, centerPoint2f.y + halfHeight);
    cv::Point2f bottomRight = cv::Point2f(centerPoint2f.x + halfWidth, centerPoint2f.y + halfHeight);

    cv::rectangle(_canvas, topLeft, bottomRight, rectColor, -1);
    return true;

    return false;
}
int PPong::paddleCollisionDetector() {
    // Calculate the difference between ball and paddle position
    cv::Point2f playerBalldifference = _ballPos - playerPaddlePos;

    // Calculate the corners of the paddle
    cv::Point2f bottomRight = playerPaddlePos + cv::Point2f(paddleWidth / 2, paddleHeight / 2);
    cv::Point2f topRight = playerPaddlePos + cv::Point2f(paddleWidth / 2, -paddleHeight / 2);
    cv::Point2f bottomLeft = playerPaddlePos + cv::Point2f(-paddleWidth / 2, paddleHeight / 2);
    cv::Point2f topLeft = playerPaddlePos + cv::Point2f(-paddleWidth / 2, -paddleHeight / 2);
    calc_end = std::chrono::steady_clock::now();
    // Check if the ball is within the horizontal bounds of the paddle
    ballDebounce = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - calc_collisionTime);
    if (playerBalldifference.x < sliderRadius + paddleWidth / 2 &&
        playerBalldifference.x > -sliderRadius - paddleWidth / 2 &&
        (ballDebounce.count()) > 500) {
        // Check if the ball is within the vertical bounds of the paddle
        if ((_ballPos.y >= topLeft.y) && (_ballPos.y <= bottomLeft.y)) {
            calc_collisionTime = std::chrono::steady_clock::now();
            std::cout << "Detection has been detected!! XY -BOUNDS\n\n";
            return 1;
        }
    }
    if (_ballPos.x - sliderRadius < 0 ) {//|| _ballPos.x +sliderRadius > matSize.width
        return 2;
    }
    return 0;
}
int PPong::aiPaddleCollisionDetector() {
    // Calculate the difference between ball and AI paddle position
    cv::Point2f aiBallDifference = _ballPos - aiPaddlePos;

    // Calculate the corners of the AI paddle
    cv::Point2f bottomLeft = aiPaddlePos + cv::Point2f(-paddleWidth / 2, paddleHeight / 2);
    cv::Point2f topLeft = aiPaddlePos + cv::Point2f(-paddleWidth / 2, -paddleHeight / 2);
    cv::Point2f bottomRight = aiPaddlePos + cv::Point2f(paddleWidth / 2, paddleHeight / 2);
    cv::Point2f topRight = aiPaddlePos + cv::Point2f(paddleWidth / 2, -paddleHeight / 2);
    ballDebounce = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now()- calc_collisionTime );
    // Check if the ball is within the horizontal bounds of the AI paddle (considering the ball's radius)

    if (-aiBallDifference.x < sliderRadius + (paddleWidth / 2 )  &&
        aiBallDifference.x > -sliderRadius - (paddleWidth / 2 ) &&
        (ballDebounce.count()) > 500) {
       // deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start);
        // Check if the ball is within the vertical bounds of the AI paddle
        if ((_ballPos.y >= topLeft.y) && (_ballPos.y <= bottomLeft.y)) {
            calc_collisionTime = std::chrono::steady_clock::now();
            return 1;
        }
    }
    if (_ballPos.x +sliderRadius> matSize.width) {
        return 2;
    }
    return 0;
}
bool PPong::createBall() {

   // velocity *= additionalVelocity;
    float deltaTimeInSeconds = deltaT.count() / 1000.0f; // Convert milliseconds to seconds

    _ballVel = _ballVel / magnitude(_ballVel);

    //velocity represents how many frames we need, delta T will dictate yeah
    //velocity = cv::Point(xDirection * sliderVelocity * deltaTimeInSeconds, 0);
    // Assuming 'velocity', '_ballPos', and 'deltaTimeInSeconds' are defined and updated properly
    cv::Point scaledVelocity = _ballVel * deltaTimeInSeconds * sliderVelocity;
    _ballPos.x = _ballPos.x + scaledVelocity.x ;
    _ballPos.y = _ballPos.y - scaledVelocity.y ;
   // std::cout << "Updated Position - X: " << position.x << ", Y: " << position.y << std::endl;
    //Now we need to detect left rectangle or right rectangle.
    /*
    if (_ballPos.x - sliderRadius < 0 || _ballPos.x + sliderRadius > matSize.width) {
        _ballVel.x *= -1;
    }
    */
    int paddleDetection = paddleCollisionDetector();
    int aiPaddleDetection = aiPaddleCollisionDetector();

    if (paddleDetection == 1) {//collision with playerpaddle
        _ballVel.x = 1;
        _ballVel.y += (rectVelocity.y * 0.05);
    }else if (paddleDetection == 2) {// player lost a point
        enemyScore++;
        if (enemyScore == 5) {
            gameOver = "You lose!";
            resetGame();
        }
        _ballPos = cv::Point2f(matSize.width / 2, matSize.height / 2);
        int randDirection = (std::rand() % 2 == 1) ? -1 : 1;
        _ballVel = cv::Point2f(10 * randDirection, std::rand() % 10);
    }else if (aiPaddleDetection == 2){ // player won a point
        playerScore++;
        if (playerScore == 5) {
            gameOver = "You Win!";
            resetGame();
        }
        _ballPos = cv::Point2f(matSize.width / 2, matSize.height / 2);
        int randDirection = (std::rand() % 2 == 1) ? -1 : 1;
        _ballVel = cv::Point2f(10 * randDirection, std::rand() % 10);
    }else if (aiPaddleDetection == 1) {//collision with aiPaddle
        _ballVel.x = -1;
        _ballVel.y += (aiPaddleVel.y * 0.05);
    }else if (_ballPos.y - sliderRadius < 0 || _ballPos.y + sliderRadius > matSize.height) {
        //we need to add some code.
        if (_ballPos.y - sliderRadius < 0) {//for glitches when parts of ball reaches out of canvas bounds.
            _ballPos.y = sliderRadius;
        }else if (_ballPos.y + sliderRadius > matSize.height) {
            _ballPos.y = matSize.height - sliderRadius;
        }
        _ballVel.y *= -1;

    }
   //std::cout << "Position: (" << _ballPos.x << ", " << _ballPos.y << "), Velocity: (" << _ballVel.x << ", " << _ballVel.y << "), Ball Radius: " << sliderRadius << std::endl;

    return true;
}
