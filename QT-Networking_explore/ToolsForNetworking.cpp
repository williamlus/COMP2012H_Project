class Player_info{
	public:

	int player_index;
	unsigned int cards_remain;
	int role;//-1 for null, 0 for landlord, 1 for farmer
	
	Player_info(player_index=-1.cards_remain=-1,role=-1) : player_index(player_index), cards_remain(cards_remain), role(role) {}
	
};

class DataPackage{
	public:

	int data_type;
	/* 0: confirm_ready
	 * 2: deal cards
	 * 1: choose landlord
	 * 3: play cards
	 */
	int sender_id; // 0 for server, 1 for client
	QVector <Card> cards;
	QVector <Player_Info> player_info;
	int active_player_id;

	DataPackage(data_type=-1, sender_id=-1, cards={}, player_info={}, active_player_id=-1) :
	data_tpe(data_type), sender_id(sender_id), cards(cards), player_info(player_info), active_player_id(active_player_id) {}

	friend QDataStream& operator>>(QDataStream& in, DataPackage& data){
		in >> data.data_type >> data.sender_id >> data.cards >> data.player_info >> data.active_player_id;
		return in;
	}

	friend QDataStream& operator<<(QDataStream& out, DataPackage& data){
		out << data.data_type << data.sender_id << data.cards << data.player_info << data.active_player_id;
		return out;
	}
};
