#ifndef __SFTIMESPAN_H__
#define __SFTIMESPAN_H__
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2016 by Great Hill Corporation.
 * All Rights Reserved
 *
 *------------------------------------------------------------------------*/

#include "sfos.h"

class SFTimeOfDay;

//----------------------------------------------------------------------------
// An SFTimeSpan class with a granularity of 1 second which
// is used in conjunction with the time class SFTime
//----------------------------------------------------------------------------
class SFTimeSpan
{
public:
											SFTimeSpan         (void);
											SFTimeSpan         (const SFTimeSpan& ts);

											SFTimeSpan         (SFInt32 d, SFInt32 h, SFInt32 m, SFInt32 s);
											SFTimeSpan         (const SFTimeOfDay& tod);
											SFTimeSpan         (SFInt64 s);

//									  ~SFTimeSpan         (void);

					SFTimeSpan& operator=          (const SFTimeSpan& ts);

					SFInt32     getTotalDays       (void) const;
					SFInt32     getDaysAbs         (void) const;
					SFInt32     getHoursAbs        (void) const;
					SFInt32     getMinutesAbs      (void) const;
					SFInt32     getSecondsAbs      (void) const;
					SFInt64		getSeconds_raw     (void) const;
					double      getSeconds_double  (void) const;

					SFBool      isPositive         (void) const;
					SFBool      isNegative         (void) const;
					SFBool      isZero             (void) const;
					SFBool      IsValid            (void) const;
					SFTimeSpan& Negate             (void);
                                           
					SFTimeSpan  operator+          (const SFTimeSpan& ts) const;
					SFTimeSpan  operator-          (const SFTimeSpan& ts) const;
  friend	SFTimeSpan  operator-          (const SFTimeSpan& ts);
					SFTimeSpan  operator*          (SFInt32 mult) const;
  friend  SFTimeSpan  operator*          (SFInt32 mult, const SFTimeSpan& ts);
					SFTimeSpan  operator/          (SFInt32 div) const;

					SFTimeSpan& operator+=         (SFTimeSpan& ts);
					SFTimeSpan& operator-=         (SFTimeSpan& ts);
					SFTimeSpan& operator*=         (SFInt32 mult);
					SFTimeSpan& operator/=         (SFInt32 div);
  
					SFBool      operator==         (const SFTimeSpan& ts) const;
					SFBool      operator!=         (const SFTimeSpan& ts) const;
					SFBool      operator>          (const SFTimeSpan& ts) const;
					SFBool      operator<          (const SFTimeSpan& ts) const;
					SFBool      operator>=         (const SFTimeSpan& ts) const;
					SFBool      operator<=         (const SFTimeSpan& ts) const;

				  SFString		Format		         (const SFString& fmt=nullString) const;

private:
  SFInt64 m_nSeconds;
};

//-------------------------------------------------------------------------
inline double SFTimeSpan::getSeconds_double() const
{
	ASSERT(IsValid());
	return (double)m_nSeconds;
}

//-------------------------------------------------------------------------
inline SFInt64 SFTimeSpan::getSeconds_raw(void) const
{
	return m_nSeconds;
}

//----------------------------------------------------------------------------
inline SFTimeSpan& SFTimeSpan::Negate()
{
	if (IsValid())
		m_nSeconds = (-m_nSeconds);
	return *this;
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::operator==(const SFTimeSpan& ts) const
{
	return (m_nSeconds == ts.m_nSeconds);
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::operator>(const SFTimeSpan& ts) const
{
	return (m_nSeconds > ts.m_nSeconds);
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::operator>=(const SFTimeSpan& ts) const
{
	return (m_nSeconds >= ts.m_nSeconds);
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::operator<(const SFTimeSpan& ts) const
{
	return (m_nSeconds < ts.m_nSeconds);
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::operator<=(const SFTimeSpan& ts) const
{
	return (m_nSeconds <= ts.m_nSeconds);
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::operator!=(const SFTimeSpan& ts) const
{
	return (m_nSeconds != ts.m_nSeconds);
}
                                 
//----------------------------------------------------------------------------
inline SFTimeSpan SFTimeSpan::operator+(const SFTimeSpan& ts) const
{
	if (IsValid() && ts.IsValid())
		return SFTimeSpan((SFInt64)(m_nSeconds + ts.m_nSeconds));
	return SFTimeSpan();
}

//----------------------------------------------------------------------------
inline SFTimeSpan SFTimeSpan::operator-(const SFTimeSpan& ts) const
{
	if (IsValid() && ts.IsValid())
		return SFTimeSpan((SFInt64)(m_nSeconds - ts.m_nSeconds));
	return SFTimeSpan();
}

//----------------------------------------------------------------------------
inline SFTimeSpan& SFTimeSpan::operator+=(SFTimeSpan& ts)
{
	*this = *this + ts;
	return *this;
}

//----------------------------------------------------------------------------
inline SFTimeSpan& SFTimeSpan::operator-=(SFTimeSpan& ts)
{
	*this = *this - ts;
	return *this;
}

//----------------------------------------------------------------------------
inline SFTimeSpan operator-(const SFTimeSpan& ts)
{
	return SFTimeSpan(ts).Negate();
}

//----------------------------------------------------------------------------
inline SFTimeSpan& SFTimeSpan::operator*=(SFInt32 mult)
{
	*this = SFTimeSpan::operator*(mult);
	return *this;
}

//----------------------------------------------------------------------------
inline SFTimeSpan SFTimeSpan::operator/(SFInt32 div) const
{
	if (div && IsValid())
		return SFTimeSpan((SFInt64)(m_nSeconds / div));
	return SFTimeSpan();
}

//----------------------------------------------------------------------------
inline SFTimeSpan& SFTimeSpan::operator/=(SFInt32 div)
{
	*this = SFTimeSpan::operator/(div);
	return *this;
}

//----------------------------------------------------------------------------
inline SFBool SFTimeSpan::IsValid() const
{
	return (m_nSeconds != (SFInt64)_I64_MIN);
}

//----------------------------------------------------------------------------
extern SFString getTimeSpanString(const SFTime& t1, const SFTime& t2);
extern SFString getTimeSpanString(const SFTimeSpan& ts);

#endif
