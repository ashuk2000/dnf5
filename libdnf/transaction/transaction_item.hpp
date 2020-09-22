/*
Copyright (C) 2017-2020 Red Hat, Inc.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef LIBDNF_TRANSACTION_TRANSACTION_ITEM_HPP
#define LIBDNF_TRANSACTION_TRANSACTION_ITEM_HPP


#include <memory>
#include <string>

#include "libdnf/utils/sqlite3/sqlite3.hpp"

namespace libdnf::transaction {
class Transaction;
class TransactionItem;
typedef std::shared_ptr< TransactionItem > TransactionItemPtr;
}

#include "comps_environment.hpp"
#include "comps_group.hpp"
#include "rpm_package.hpp"
#include "transaction_item_action.hpp"
#include "transaction_item_reason.hpp"
#include "transaction_item_state.hpp"
#include "transaction_item_type.hpp"


namespace libdnf::transaction {


class TransactionItem {
public:
    using Action = TransactionItemAction;
    using Reason = TransactionItemReason;
    using State = TransactionItemState;
    using Type = TransactionItemType;

    explicit TransactionItem(Transaction & trans);

    ItemPtr getItem() const noexcept { return item; }
    void setItem(ItemPtr value) { item = value; }

    int64_t get_id() const noexcept { return id; }
    void set_id(int64_t value) { id = value; }

    Action get_action() const noexcept { return action; }
    void set_action(Action value) { action = value; }
    std::string get_action_name();
    std::string get_action_short();

    Reason get_reason() const noexcept { return reason; }
    void set_reason(Reason value) { reason = value; }

    State get_state() const noexcept { return state; }
    void set_state(State value) { state = value; }

    const std::string & get_repoid() const noexcept { return repoid; }
    void set_repoid(const std::string & value) { repoid = value; }

    /// Has the item appeared on the system during the transaction?
    bool is_forward_action() const;

    /// Has the item got removed from the system during the transaction?
    bool is_backward_action() const;


    uint32_t getInstalledBy() const;

    // int64_t getTransactionId() const noexcept { return trans.getId(); }

    const std::vector< TransactionItemPtr > &getReplacedBy() const noexcept { return replacedBy; }
    void addReplacedBy(TransactionItemPtr value) { if (value) replacedBy.push_back(value); }

    void save();
    void saveReplacedBy();
    void saveState();

protected:
    int64_t id = 0;
    Action action = Action::INSTALL;
    Reason reason = Reason::UNKNOWN;
    State state = State::UNKNOWN;
    std::string repoid;

    Transaction & trans;

    ItemPtr item;

    // TODO: replace with objects? it's just repoid, probably not necessary
    std::vector< TransactionItemPtr > replacedBy;

    void dbInsert();
    void dbUpdate();
};


}  // namespace libdnf::transaction


#endif  // LIBDNF_TRANSACTION_TRANSACTION_ITEM_HPP
