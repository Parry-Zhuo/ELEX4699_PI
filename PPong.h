/**
 * @file PPong.h
 * @brief Defines the PPong class for the Pong game, inheriting from CBase4618.
 *
 * Creates a PPONG game class..
 */

#pragma once
#include <thread>
#include "CBase4618.h"
#include <opencv2/highgui.hpp> // For imshow

 /**
  * @class PPong
  * @brief Creates a PONG GAME where players can lose to the enemy paddle. Made using OPENCV. First competitor to get 5 points wins game.
  * Difficulty of game can be tweaked.
  *
  */
class PPong : public CBase4618 {
public:
    /**
     * @brief Constructor for PPong, initializing the game with a canvas and COM port.
     *
     * Sets up the game canvas based on the provided dimensions and initializes communication
     * with the control hardware using the specified COM port. It also sets initial positions
     * and velocities for the ball and paddles, and initializes scores and game state.
     *
     * @param canvasSize The dimensions of the game canvas as a cv::Size object.
     * @param comPort The COM port number for communication with external control hardware.
     */
    PPong(cv::Size canvasSize, int comPort);

    /**
     * @brief Updates the game state by reading input from the control hardware.
     *
     * This method should be overridden in derived classes to implement game-specific logic for
     * updating the state based on user input and internal calculations.
     */
    void update() override;
    /**
     * @brief Draws the game state to the canvas, including the GUI, paddles, and ball.
     *
     * Renders the current game state to the OpenCV canvas. This includes drawing the paddles,
     * the ball, and the GUI elements like scores and buttons. It updates the display at a fixed
     * interval to maintain consistent frame rates.
     */

    void draw() override;
    /**
     * @brief Creates a rectangular paddle on the canvas at a specified location.
     *
     * Draws a rectangle to represent a paddle on the canvas. The rectangle's position and size
     * are determined by the provided center point, width, and height.
     *
     * @param centerPoint2f The center point of the rectangle.
     * @param rectWidth The width of the rectangle.
     * @param rectHeight The height of the rectangle.
     * @return True if the rectangle was successfully created; otherwise, false.
     */
    bool createRectangle(cv::Point2f& centerPoint2f, int rectWidth, int rectHeight);
    void createControlPaddle(cv::Point2f& position, cv::Point2f& velocity,int paddleWidth, int paddleHeight);
    /**
     * @brief Updates the ball's position based on its velocity and detects collisions.
     *
     * Moves the ball according to its current velocity, checking for collisions with paddles
     * and walls. It adjusts the ball's velocity and position as necessary and updates scores
     * and game state based on collision outcomes.
     *
     * @return bool True if the ball's position was successfully updated, false otherwise.
     */
    bool createBall();

    /**
     * @brief Detects if a point has hit the boundary walls.
     *
     * Adjusts the position vector if it goes beyond the game boundaries, ensuring that objects
     * remain within the visible area of the canvas. It returns a boolean indicating whether the
     * position was adjusted due to collision with the wall.
     *
     * @param positionVector The position vector of the object to check.
     * @return bool True if the object's position is within boundaries after adjustment, false if it was adjusted.
     */

    bool pointWallDetection(cv::Point2f& positionVector);
    /**
     * @brief Detects collisions between the ball and paddles.
     *
     * Checks for collisions between the ball and both the player and AI paddles. It updates
     * the game state based on the collision outcome, such as reversing the ball's direction
     * or updating scores. The method returns an integer indicating the type of collision.
     *
     * @return int 0 if no collision, 1 for paddle collision, 2 if a round is over.
     */
    int paddleCollisionDetector();
    /**
     * @brief Detects collision between the ball and the AI paddle.
     *
     * Similar to paddleCollisionDetector, but specifically checks for collisions with the AI paddle.
     * Used to handle game logic for AI paddle interactions.
     *
     * @return An integer indicating the collision result: 0 for no collision, 1 for AI paddle collision, and 2 if the round is over.
     */
    int aiPaddleCollisionDetector();
    /*
    * @brief
    */
    bool addYposition(cv::Point2f& positionVector, cv::Point2f& velocityVector);
    float magnitude(const cv::Point2f& point);
    /**
     * @brief Draws the game's graphical user interface.
     *
     * Renders the game's current score, the FPS counter, and control buttons on the screen.
     * This method updates the canvas with game statistics and options for the player.
     */
    void drawGUI();


    /**
     * @brief Resets the game to its initial state.
     *
     * Resets scores, ball, and paddle positions to their starting values, and prepares the game
     * for a new round. This method is typically called after a game ends, either by winning,
     * losing, or pressing the reset button.
     */
    void resetGame();
    /**
     * @brief Starts the game loop, initializing threads for updating and drawing.
     *
     * Launches separate threads for handling game updates and drawing to ensure smooth gameplay.
     * The game continues running until the 'q' key is pressed to quit.
     */
    void start();
    /**
    * @brief Gracefully ends the game, ensuring all threads are properly joined.
    *
    * Stops the game loop, joins all running threads, and performs necessary cleanup operations.
    * This method is called when the game is exited.
    */
    void end();
//    void run();

private:
    char key; ///< Key input for controlling the game flow.
    cv::Point2f _ballPos; ///< Current position of the ball.
    cv::Point2f aiPaddlePos; ///< Position of the AI paddle.
    cv::Point2f playerPaddlePos; ///< Position of the player's paddle.

    cv::Point2f _ballVel; ///< Velocity of the ball.
    cv::Point2f aiPaddleVel; ///< Velocity of the AI paddle.
    cv::Point2f rectVelocity; ///< Velocity of the player's paddle.

    int paddleWidth; ///< Width of the paddles.
    int paddleHeight; ///< Height of the paddles.
    int playerScore; ///< Player's current score.
    int enemyScore; ///< AI's current score.

    int sliderVelocity; ///< Slider control for adjusting the velocity.
    int sliderRadius; ///< Slider control for adjusting the radius of the ball.

    bool _thread_exit; ///< Flag to control the exit of threads.
    bool gamePause;/// pauses game.
    bool isThreading;
    cv::Size matSize; ///< Size of the canvas.

    std::chrono::steady_clock::time_point calc_start; ///< Start time point for frame calculation.
    std::chrono::steady_clock::time_point calc_end; ///< End time point for frame calculation.
    std::chrono::milliseconds deltaT; ///< Time difference for frame rate calculation.

    std::chrono::steady_clock::time_point calc_collisionTime; ///< Time point for tracking collision debounce.
    std::chrono::milliseconds ballDebounce; ///< Debounce time to prevent immediate re-collision.

    std::thread t1; ///< Thread for handling game updates.
    std::thread t2; ///< Thread for handling game drawing.

    std::string gameOver; ///< Message displayed when the game is over.

    /**
     * @brief Static function to handle game updates in a separate thread.
     * @param ptr Pointer to the PPong object to update.
     */
    static void update_thread(PPong* ptr);

    /**
     * @brief Static function to handle game drawing in a separate thread.
     * @param ptr Pointer to the PPong object to draw.
     */
    static void draw_thread(PPong* ptr);

};
