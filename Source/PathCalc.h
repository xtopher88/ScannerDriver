/*
 * PathCalc.h
 *
 *  Created on: May 25, 2015
 *      Author: kchristopher
 */

#ifndef SOURCE_PATHCALC_H_
#define SOURCE_PATHCALC_H_


/// point move to a point as quickly as possible

/// line path, follow a given line to move to a point

/// arc path, follow a given arc to a point

/// path global parameters
///  - Target Velocity mm/sec
///  - Laser Power, intensity/sec
///  - Acceleration Setting  mm/(sec^2)

/// need velocity matching of previous and next paths
/// - at start to stop point, could load the next path
/// - calculate the stop, start velocities and the new transition point
/// -
/// needs laser power correction based on current velocity

/// run the path calculations in an interrupt above the main loop and serial communications, but under the PID control loop


/*
 * Line Path
 * -input G1 Xnnn Ynnn Znnn Ennn Fnnn Snnn
 */



/*
 * Arc Path
 *  - input G2 Xnnn Ynnn Innn Jnnn Ennn (Clockwise Arc)
 *  - input G3 Xnnn Ynnn Innn Jnnn Ennn (Counter-Clockwise Arc)
 *
 *  calculate
 *   - start xy velocity vector for velocity matching from previous path
 *     - given the final position of the last path and center point (I,J)
 *       - calculate the start radius and angle
 *       - calculate the max starting velocities and match with the finish velocity of the current movement
 *         - impose a x and y max acceleration on the transition,
 *         - ?? should some path deviation be allowed, in he case where there is no velocity angle change the solution is obvious
 *   - start and finish radius, and linear radius to angle calculation
 *   - start and finish angles
 *   - movement time profile and angle phases
 *   - stop
 *   - stop evaluation criteria, then to check for velocity matching and or start to stop the movement
 *
 *
 */




#endif /* SOURCE_PATHCALC_H_ */
