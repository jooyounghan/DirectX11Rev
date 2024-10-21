#include "AObjectManageModal.h"

using namespace std;

AObjectManageModal::AObjectManageModal(
	const string& ModalHeaderNameIn,
	EditorWorld* EditorWorldIn
)
	: AModal(ModalHeaderNameIn), EditorWorldCached(EditorWorldIn)
{
}

AObjectManageModal::~AObjectManageModal()
{
}
