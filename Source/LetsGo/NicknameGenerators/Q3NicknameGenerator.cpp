#include "Q3NicknameGenerator.h"

Q3NicknameGenerator::Q3NicknameGenerator()
{
	//TODO: import from csv

	_nicknames.Add("Crash");
	_nicknames.Add("Ranger");
	_nicknames.Add("Phobos");
	_nicknames.Add("Mynx");
	_nicknames.Add("Orbb");
	_nicknames.Add("Sarge");
	_nicknames.Add("Bitterman");
	_nicknames.Add("Grunt");
	_nicknames.Add("Hossman");
	_nicknames.Add("Daemia");
	_nicknames.Add("Hunter");
	_nicknames.Add("Angel");
	_nicknames.Add("Gorre");
	_nicknames.Add("Klesk");
	_nicknames.Add("Slash");
	_nicknames.Add("Wrack");
	_nicknames.Add("Biker");
	_nicknames.Add("Lucy");
	_nicknames.Add("Patriot");
	_nicknames.Add("Tank Jr.");
	_nicknames.Add("Anarki");
	_nicknames.Add("Stripe");
	_nicknames.Add("Razor");
	_nicknames.Add("Keel");
	_nicknames.Add("Visor");
	_nicknames.Add("Uriel");
	_nicknames.Add("Bones");
	_nicknames.Add("Cadaver");
	_nicknames.Add("Sorlag");
	_nicknames.Add("Doom");
	_nicknames.Add("Major");
	_nicknames.Add("Xaero");
}

FName Q3NicknameGenerator::Generate()
{
	auto const nickname = TakeRandom(_nicknames);
	return FName(nickname);
}
