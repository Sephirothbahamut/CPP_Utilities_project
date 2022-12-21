#pragma once

//#define utils_virtualize   protected: virtual void utils_i_am_virtual() = 0;
//#define utils_devirtualize protected: virtual void utils_i_am_virtual() final override { };
namespace utils::oop
	{
	class devirtualize;

	/// <summary>
	/// This class should be inherited virtually.
	/// </summary>
	class virtualize 
		{
		friend class devirtualize;
		virtual void utils_i_am_virtual() = 0; 
		};
	class devirtualize : virtual virtualize 
		{
		virtual void utils_i_am_virtual() final override {} 
		};
	}