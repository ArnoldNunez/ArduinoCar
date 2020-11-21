#ifndef GEM_H
#define GEM_H


namespace ArduinoCar_Core
{
	/**
	 * Class representing a Gem
	 */
	class Gem
	{
	public:
		double X;	// X coordinate
		double Y;	// Y coordinate
		char Type;	// The type of gem (A - Z)
		
		/**
		 * Constructor
		 */
		Gem(int id, double x, double y, char type);

		/** 
		 * Destructor
		 */
		virtual ~Gem();

		/**
		 * Sets the position of the gem
		 */
		void SetPosition(double x, double y);

		/**
		 * Sets the type of the gem
		 */
		void SetType(char type);

		/**
		 * Getter for the gem id
		 */
		int GetID() const { return this->mID; }


		bool operator == (const Gem& other) const { return other.GetID() == this->GetID(); }
		bool operator != (const Gem& other) const { return !operator==(other); }

	private:
		int mID;
	};
}



#endif // !GEM_H
