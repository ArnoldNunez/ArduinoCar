#ifndef POINT2D_H
#define POINT2D_H

namespace ArduinoCar_Core
{
	class Point2D
	{
	public:
		double X;
		double Y;


		/**
		 * Default constructor
		 */
		Point2D();

		/**
		 * Constructor
		 * \param x X coordinate of the point
		 * \param y Y coordinate of the point
		 */
		Point2D(double x, double y);

		/**
		 * Default destructor
		 */
		~Point2D();

		/**
		 * Default copy constructor
		 */
		Point2D(const Point2D& pt);
	};
}


#endif // !POINT2D_H
