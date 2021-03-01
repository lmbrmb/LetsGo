#pragma once

////<summary>
/// [Abstract] Item meta data. Stateless.
///</summary>
class LETSGO_API Item
{
public:
	explicit Item(const FName id);
	
	virtual ~Item() = default;
	
	FName GetId() const;
	
private:
	FName _id;
};
