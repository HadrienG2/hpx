//  Copyright (c) 2007-2014 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//  Copyright (c)      2011 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file base_action.hpp

#if !defined(HPX_RUNTIME_ACTIONS_BASE_ACTION_HPP)
#define HPX_RUNTIME_ACTIONS_BASE_ACTION_HPP

#include <hpx/config.hpp>
#include <hpx/runtime/naming_fwd.hpp>
#include <hpx/runtime/parcelset_fwd.hpp>
#include <hpx/runtime/actions/continuation_fwd.hpp>
#include <hpx/runtime/serialization/serialization_fwd.hpp>
#include <hpx/runtime/serialization/detail/polymorphic_intrusive_factory.hpp>
#include <hpx/runtime/threads/thread_data_fwd.hpp>
#include <hpx/traits/polymorphic_traits.hpp>

#include <boost/cstdint.hpp>

#include <memory>

namespace hpx { namespace actions
{
    ///////////////////////////////////////////////////////////////////////////
    /// The \a base_action class is an abstract class used as the base class
    /// for all action types. It's main purpose is to allow polymorphic
    /// serialization of action instances through a unique_ptr.
    struct HPX_API_EXPORT base_action
    {
        /// The type of an action defines whether this action will be executed
        /// directly or by a HPX-threads
        enum action_type
        {
            plain_action = 0, ///< The action will be executed by a newly created thread
            direct_action = 1 ///< The action needs to be executed directly
        };

        /// Destructor
        virtual ~base_action() {}

        /// The function \a get_component_type returns the \a component_type
        /// of the component this action belongs to.
        virtual int get_component_type() const = 0;

        /// The function \a get_action_name returns the name of this action
        /// (mainly used for debugging and logging purposes).
        virtual char const* get_action_name() const = 0;

        /// The function \a get_action_type returns whether this action needs
        /// to be executed in a new thread or directly.
        virtual action_type get_action_type() const = 0;

        /// The \a get_thread_function constructs a proper thread function for
        /// a \a thread, encapsulating the functionality and the arguments
        /// of the action it is called for.
        ///
        /// \param lva    [in] This is the local virtual address of the
        ///               component the action has to be invoked on.
        ///
        /// \returns      This function returns a proper thread function usable
        ///               for a \a thread.
        ///
        /// \note This \a get_thread_function will be invoked to retrieve the
        ///       thread function for an action which has to be invoked without
        ///       continuations.
        virtual threads::thread_function_type
            get_thread_function(naming::address_type lva) = 0;

        /// The \a get_thread_function constructs a proper thread function for
        /// a \a thread, encapsulating the functionality, the arguments, and
        /// the continuations of the action it is called for.
        ///
        /// \param cont   [in] This is the list of continuations to be
        ///               triggered after the execution of the action
        /// \param lva    [in] This is the local virtual address of the
        ///               component the action has to be invoked on.
        ///
        /// \returns      This function returns a proper thread function usable
        ///               for a \a thread.
        ///
        /// \note This \a get_thread_function will be invoked to retrieve the
        ///       thread function for an action which has to be invoked with
        ///       continuations.
        virtual threads::thread_function_type
            get_thread_function(std::unique_ptr<continuation> cont,
                naming::address_type lva) = 0;

        /// return the id of the locality of the parent thread
        virtual boost::uint32_t get_parent_locality_id() const = 0;

        /// Return the thread id of the parent thread
        virtual threads::thread_id_repr_type get_parent_thread_id() const = 0;

        /// Return the thread phase of the parent thread
        virtual boost::uint64_t get_parent_thread_phase() const = 0;

        /// Return the thread priority this action has to be executed with
        virtual threads::thread_priority get_thread_priority() const = 0;

        /// Return the thread stacksize this action has to be executed with
        virtual threads::thread_stacksize get_thread_stacksize() const = 0;

        /// Return whether the embedded action is part of termination detection
        virtual bool does_termination_detection() const = 0;

        /// Return all data needed for thread initialization
        virtual void schedule_thread(naming::id_type const& target,
            naming::address_type lva,
            threads::thread_state_enum initial_state, std::size_t num_thred) = 0;

        virtual void schedule_thread(std::unique_ptr<continuation> cont,
            naming::id_type const& target, naming::address_type lva,
            threads::thread_state_enum initial_state, std::size_t num_thred) = 0;

        /// Return a pointer to the filter to be used while serializing an
        /// instance of this action type.
        virtual serialization::binary_filter* get_serialization_filter(
            parcelset::parcel const& p) const = 0;

        /// Return a pointer to the message handler to be used for this action.
        virtual parcelset::policies::message_handler* get_message_handler(
            parcelset::parcelhandler* ph, parcelset::locality const& loc,
            parcelset::parcel const& p) const = 0;

#if defined(HPX_HAVE_SECURITY)
        /// Return the set of capabilities required to invoke this action
        virtual components::security::capability get_required_capabilities(
            naming::address_type lva) const = 0;
#endif

        template <typename Archive>
        void serialize(Archive &, unsigned)
        {}

        HPX_SERIALIZATION_POLYMORPHIC_ABSTRACT(base_action);
    };
}}

HPX_TRAITS_SERIALIZED_WITH_ID(hpx::actions::base_action)

#endif
